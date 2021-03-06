//
//  Key.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <iostream>
#include "mainlineKey.h"
#include "bone.h"
#include "boneRef.h"
#include "object.h"
#include "objectRef.h"
#include "animation.h"
#include "file.h"

using namespace std;
using namespace tinyxml2;

MainlineKey::~MainlineKey() {
    for(vector<BoneRef*>::iterator it = m_boneRefs.begin(); it != m_boneRefs.end(); it++) {
        delete (*it);
    }
    m_boneRefs.empty();
    for(vector<ObjectRef*>::iterator it = m_objectRefs.begin(); it != m_objectRefs.end(); it++) {
        delete (*it);
    }
    m_objectRefs.empty();
}

void MainlineKey::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("time");
    if(attb) {
        m_time = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("spin");
    if(attb) {
        m_spin = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("curve_type");
    if(attb) {
        m_curve_type = attb->Value();
    }
    
    attb = a_element->FindAttribute("c1");
    if(attb) {
        m_c1 = attb->FloatValue();
    }
    
    attb = a_element->FindAttribute("c2");
    if(attb) {
        m_c2 = attb->FloatValue();
    }
    
    attb = a_element->FindAttribute("c3");
    if(attb) {
        m_c3 = attb->FloatValue();
    }
    
    attb = a_element->FindAttribute("c4");
    if(attb) {
        m_c4 = attb->FloatValue();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while(child) {
        if(strcmp(child->Name(), "bone_ref") == 0) {
            BoneRef* boneRef = new BoneRef();
            boneRef->loadXML(child);
            addBoneRef(boneRef);
        }
        else if(strcmp(child->Name(), "object_ref") == 0) {
            ObjectRef* objectRef = new ObjectRef();
            objectRef->loadXML(child);
            addObjectRef(objectRef);
        }        child = child->NextSiblingElement();
    }
}

void MainlineKey::addBoneRef(BoneRef* a_boneRef) {
    m_boneRefs.push_back(a_boneRef);
}

void MainlineKey::addObjectRef(ObjectRef* a_objectRef) {
    m_objectRefs.push_back(a_objectRef);
}

ObjectRef* MainlineKey::findReferenceToObject(int a_timelineIndex, unsigned int a_keyIndex) {
    for(vector<ObjectRef*>::iterator it = m_objectRefs.begin(); it != m_objectRefs.end(); ++it) {
        if((*it)->getTimeline() == a_timelineIndex && (*it)->getKey() == a_keyIndex)
            return *it;
    }
    return NULL;
}

ObjectRef* MainlineKey::findReferenceToObject(int a_timelineIndex) {
    for(vector<ObjectRef*>::iterator it = m_objectRefs.begin(); it != m_objectRefs.end(); ++it) {
        if((*it)->getTimeline() == a_timelineIndex)
            return *it;
    }
    return NULL;
}

// find the bone reference that this object reference is referring to
BoneRef* MainlineKey::getBoneReference(ObjectRef* a_objectRef) {
    int idx = 0;
    for(vector<ObjectRef*>::iterator it = m_objectRefs.begin(); it != m_objectRefs.end(); ++it, ++idx) {
        if(a_objectRef->getTimeline() == (*it)->getTimeline() && (*it)->getParent() >= 0) {
            return m_boneRefs[(*it)->getParent()];
        }
    }
    return NULL;
}

BoneRef* MainlineKey::getBoneReference(unsigned int a_id) {
    if(a_id >= m_boneRefs.size())
        return NULL;
    return m_boneRefs[a_id];
}

MainlineKey* MainlineKey::clone() {
    MainlineKey* mainlineKey = new MainlineKey();
    mainlineKey->setOwner(this->getOwner());
    mainlineKey->setId(this->getId());
    mainlineKey->setSpin(this->getSpin());
    mainlineKey->setTime(this->getTime());
    return mainlineKey;
}





