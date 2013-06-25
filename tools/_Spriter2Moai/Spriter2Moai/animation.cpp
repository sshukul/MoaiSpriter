//
//  Animation.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include "animation.h"
#include "mainlineKey.h"
#include "timeline.h"
#include "entity.h"
#include "spriterData.h"
#include "file.h"
#include "objectRef.h"
#include "boneRef.h"

using namespace std;
using namespace tinyxml2;
using namespace boost::filesystem;

Animation::~Animation() {
    for (vector<MainlineKey*>::iterator it = m_mainlineKeys.begin(); it != m_mainlineKeys.end(); it++) {
        delete (*it);
    }
    m_mainlineKeys.clear();
    
    for (vector<Timeline*>::iterator it = m_timelines.begin(); it != m_timelines.end(); it++) {
        delete (*it);
    }
    m_timelines.clear();
}

void Animation::addMainlineKey(MainlineKey* a_key) {
    m_mainlineKeys.push_back(a_key);
}

void Animation::addTimeline(Timeline* a_timeline) {
    m_timelines.push_back(a_timeline);
}

Bone* Animation::getBone(unsigned int a_timelineIndex, unsigned int a_keyIndex) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("bone") == 0) {
        return m_timelines[a_timelineIndex]->getBone(a_keyIndex);
    }
    return NULL;
}

Object* Animation::getObject(unsigned int a_timelineIndex, unsigned int a_keyIndex) {    
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("object") == 0) {
        return m_timelines[a_timelineIndex]->getObject(a_keyIndex);
    }
    return NULL;
}

string Animation::getFileName(unsigned int a_folderIndex, unsigned int a_fileIndex) {
    SpriterData* spriter = m_owner->getOwner();
    
    string szPath = spriter->getFilePath(a_folderIndex, a_fileIndex);
    if(szPath.length() <= 0)
        return szPath;
    
    path file(szPath);
    return file.filename().c_str();
}

File* Animation::getFile(unsigned int a_folderIndex, unsigned int a_fileIndex) {
    SpriterData* spriter = m_owner->getOwner();
    
    return spriter->getFile(a_folderIndex, a_fileIndex);
}

ObjectRef* Animation::findReferenceToObject(unsigned int a_timelineIndex, unsigned int a_keyIndex, int* keyId) {
    int index = 0;
    for(vector<MainlineKey*>::iterator it = m_mainlineKeys.begin(); it != m_mainlineKeys.end(); ++it, ++index) {
        ObjectRef* ref = (*it)->findReferenceToObject(a_timelineIndex, a_keyIndex);
        if(ref != NULL) {
            if(keyId != NULL)
                *keyId = index;
            return ref;
        }
    }
    return NULL;
}

BoneRef* Animation::getBoneReference(ObjectRef* a_objectRef, int a_keyId) {
    if(a_keyId >= m_mainlineKeys.size())
        return NULL;
    return m_mainlineKeys[a_keyId]->getBoneReference(a_objectRef);
}

BoneRef* Animation::getBoneReference(unsigned int a_boneRefId, unsigned int a_keyId) {
    if(a_keyId >= m_mainlineKeys.size())
        return NULL;
    return m_mainlineKeys[a_keyId]->getBoneReference(a_boneRefId);
}

void Animation::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
    
    attb = a_element->FindAttribute("length");
    if(attb) {
        m_length = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("looping");
    if(attb) {
        m_looping = attb->BoolValue();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while (child) {
        if(strcmp(child->Name(), "mainline") == 0) {
            const tinyxml2::XMLElement* mainlineChild = child->FirstChildElement();
            while (mainlineChild) {
                if(strcmp(mainlineChild->Name(), "key") == 0) {
                    MainlineKey* key = new MainlineKey(this);
                    key->loadXML(mainlineChild);
                    addMainlineKey(key);
                }
                mainlineChild = mainlineChild->NextSiblingElement();
            }
        }
        else if(strcmp(child->Name(), "timeline") == 0) {
            Timeline* timeline = new Timeline(this);
            timeline->loadXML(child);
            addTimeline(timeline);
        }
        child = child->NextSiblingElement();
    }
}

std::ostream& operator<< (std::ostream& out, const Animation& animation) {
    out << "\t['" << animation.m_name << "'] = {" << endl;
    
    // count how many object timelines we have
    int objectCnt = 0;
    for(vector<Timeline*>::const_iterator it = animation.m_timelines.begin(); it != animation.m_timelines.end(); ++it) {
        if((*it)->isTypeObject())
            objectCnt++;
    }
    
    int counter = 0;
    for(vector<Timeline*>::const_iterator it = animation.m_timelines.begin(); it != animation.m_timelines.end(); ++it, ++counter) {
        if((*it)->isTypeObject()) {
            
            out << *(*it);
        
            if(counter < objectCnt-1)
                out << ",";
            out << endl;
        }
    }
    out << "\t}";
    return out;
}


