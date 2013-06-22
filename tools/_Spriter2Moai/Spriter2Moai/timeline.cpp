//
//  Timeline.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <iostream>
#include <boost/format.hpp>
#include <math.h>
#include "timeline.h"
#include "object.h"
#include "bone.h"
#include "animation.h"
#include "objectRef.h"
#include "boneRef.h"
#include "file.h"
#include "transform.h"

using namespace std;
using namespace tinyxml2;

Timeline::~Timeline() {
    
}

void Timeline::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
    
    attb = a_element->FindAttribute("object_type");
    if(attb) {
        m_objectType = attb->Value();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while(child) {
        if(strcmp(child->Name(), "key") == 0) {
            if (m_objectType.compare("object") == 0) {
                Object* object = new Object();
                object->loadXML(child);
                addObject(object);
            }
            else if(m_objectType.compare("bone")==0) {
                Bone* bone = new Bone();
                bone->loadXML(child);
                addBone(bone);
            }
        }
        child = child->NextSiblingElement();
    }
}

Transform Timeline::buildTransform(BoneRef* boneRef, int key) const {
    Bone* bone = m_owner->getBone(boneRef->getTimeline(), boneRef->getKey());
    Transform boneTransform(bone->getX(), bone->getY(), bone->getAngle(), bone->getScaleX(), bone->getScaleY());
    
    if(boneRef->getParent() != -1) {
        BoneRef* parent = m_owner->getBoneReference(boneRef->getParent(), key);
        Transform parentTransform = buildTransform(parent, key);
        boneTransform.apply_parent_transform(parentTransform);
    }
    
    return boneTransform;
}

// this is the trickest part in the program, because we need to output the objects in each timeline (each timeline
// describes one object), but we need to keep refering back to the mainline to see if the object is attached to any
// bones, and if so, calculate how this affects the final position of the object
// Note that we're only interested in exporting objects - not bones
std::ostream& operator<< (std::ostream& out, const Timeline& timeline) {
    
    if(!timeline.isTypeObject())
        return out;
    
    out << "\t\t[" << timeline.m_id + 1 << "] = {" << endl;
    for(vector<Object*>::const_iterator it = timeline.m_objects.begin(); it != timeline.m_objects.end(); ) {
        // For each object we have to check if it is attached to a bone. If it is, then
        // we need to recursively look up the bone's properties and calculate the values to add to the
        // object position.
        
        Object* object = *it;
        
        // search the mainline for any references to this timeline and key pair
        int mainlineKeyId;
        ObjectRef* objectRef = timeline.m_owner->findReferenceToObject(timeline.m_id, object->getId(), &mainlineKeyId);
                
        
        Transform objectTransform(object->getX(), object->getY(), object->getAngle(), object->getScaleX(), object->getScaleY());
        BoneRef* boneRef = timeline.m_owner->getBoneReference(objectRef, mainlineKeyId);
        Transform parentTransform = timeline.buildTransform(boneRef, mainlineKeyId);
        objectTransform.apply_parent_transform(parentTransform);
        
        float x = objectTransform.x;
        float y = objectTransform.y;
        int z = objectRef->getZIndex();
        float angle = objectTransform.angle;
        float scaleX = objectTransform.scale_x;
        float scaleY = objectTransform.scale_y;
                
        out << "\t\t\t[" << object->getId() + 1 << "] = {" << endl;
                    
        out << "\t\t\t\t['angle'] = " << boost::format("%.4f") % angle << "," << endl;
        out << "\t\t\t\t['texture'] = '" << timeline.m_owner->getFileName(object->getFolder(), object->getFile()) << "'," << endl;
        out << "\t\t\t\t['zindex'] = " << z << "," << endl;
        out << "\t\t\t\t['scale_x'] = " << boost::format("%.4f") % scaleX << "," << endl;
        out << "\t\t\t\t['scale_y'] = " << boost::format("%.4f") % scaleY << "," << endl;
        out << "\t\t\t\t['time'] = " << object->getTime() << "," << endl;
        out << "\t\t\t\t['x'] = " << boost::format("%.6f") % x << "," << endl;
        out << "\t\t\t\t['y'] = " << boost::format("%.6f") % y << "," << endl;
        out << "\t\t\t\t['spin'] = " << object->getSpin() << "," << endl;
        out << "\t\t\t\t['pivot_x'] = " << 0 << "," << endl;
        out << "\t\t\t\t['pivot_y'] = " << timeline.m_owner->getFile(object->getFolder(), object->getFile())->getHeight()<< endl;
        
        out << "\t\t\t}";
        if(++it != timeline.m_objects.end())
            out << ", ";
        out << endl;
    }
    out << "\t\t}";
    return out;
}

void Timeline::addObject(Object* a_object) {
    if(!isTypeObject())
        return;
    m_objects.push_back(a_object);
}

void Timeline::addBone(Bone* a_bone) {
    if(isTypeObject())
        return;
    m_bones.push_back(a_bone);
}

Object* Timeline::getObject(int a_index) {
    if(a_index >= m_objects.size())
        return NULL;
    if(!isTypeObject())
        return NULL;
    return m_objects[a_index];
}

Bone* Timeline::getBone(int a_index) {
    if(a_index >= m_bones.size())
        return NULL;
    if(isTypeObject())
        return NULL;
    return m_bones[a_index];
}





