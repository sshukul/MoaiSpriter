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
#include "mainlineKey.h"
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

float Timeline::calculateActualRotationAngle(float startAngle, float endAngle, int spin) {
    if(endAngle < startAngle) {
        if(spin == -1) {
            return endAngle - startAngle;
        } else {
            return (360 - startAngle) + endAngle;
        }
    } else if(endAngle == startAngle) {
        return 0.0;
    } else {
        if(spin == -1) {
            return endAngle - 360 - startAngle;
        } else {
            return endAngle - startAngle;
        }
    }
}

Transform Timeline::buildTransform(BoneRef* boneRef, int key, int time, int length, bool looping) const {
    Bone* bone = m_owner->getBoneByTime(boneRef->getTimeline(), time);
    if(boneRef->getKey() != key) {
        bone = m_owner->getBone(boneRef->getTimeline(), boneRef->getKey());
    }
    Transform boneTransform(0, 0, 0, 0, 0, 0);
    if(bone != NULL) {
        boneTransform = Transform(bone->getX(), bone->getY(), bone->getAngle(), bone->getScaleX(), bone->getScaleY(), bone->getSpin());
        Transform boneNextKeyTransform(bone->getX(), bone->getY(), bone->getAngle(), bone->getScaleX(), bone->getScaleY(), bone->getSpin());
        
        Bone* boneNextKey = m_owner->getNextBoneByTime(boneRef->getTimeline(), time);
        if(time != bone->getTime() && boneNextKey != NULL && boneNextKey->getTime() != bone->getTime()) {
            float nextFrameTime = boneNextKey->getTime();
            if(!(looping == false && nextFrameTime == 0)) {
                if(nextFrameTime == 0) {
                    nextFrameTime = length;
                }
                float averagingFactor = ((float)time - (float)bone->getTime()) / (nextFrameTime - (float)bone->getTime());
                Transform nextKeyTransform(boneNextKey->getX(), boneNextKey->getY(), boneNextKey->getAngle(), boneNextKey->getScaleX(), boneNextKey->getScaleY(), boneNextKey->getSpin());
                boneTransform.lerp(nextKeyTransform, averagingFactor, bone->getSpin());
            }
        }
        

        
        if(boneNextKey != NULL && boneNextKey->getTime() != bone->getTime()) {
            int nextMainlineKeyTime = 0;
            for(vector<MainlineKey*>::const_iterator it = m_owner->m_mainlineKeys.begin(); it != m_owner->m_mainlineKeys.end(); it++) {
                if((*it)->getTime() == time) {
                    if(it+1 != m_owner->m_mainlineKeys.end()) {
                        it++;
                        nextMainlineKeyTime = (*it)->getTime();
                        break;
                    }
                }
            }
            
            // Find the bone angle at the next keyframe, based on lerping if needed. Use it
            // to calculate the rotation angle and spin for the bone.
            if(boneNextKey->getTime() != nextMainlineKeyTime) {
                int nextKeyTime = boneNextKey->getTime();
                bool skipLerp = false;
                if(boneNextKey->getTime() == 0) {
                    nextKeyTime = length;
                    if(looping == false) {
                        skipLerp = true;
                    }
                }
                // If there is no next bone keyframe and we are not looping, then there is no rotation for this
                // bone. Otherwise calculate linear interpolation to find the rotation angle for the
                // next keyframe.
                if(!skipLerp) {
                    float averagingFactor = ((float)nextMainlineKeyTime - (float) bone->getTime()) / ((float) nextKeyTime - (float) bone->getTime());
                    Transform nextKeyTransform(boneNextKey->getX(), boneNextKey->getY(), boneNextKey->getAngle(), boneNextKey->getScaleX(), boneNextKey->getScaleY(), boneNextKey->getSpin());
                    boneNextKeyTransform.lerp(nextKeyTransform, averagingFactor, bone->getSpin());
                    boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, boneNextKeyTransform.angle, bone->getSpin());
                } else {
                    boneTransform.rotationAngle = 0;
                    boneTransform.spin = 0;
                }
            } else {
                boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, boneNextKey->getAngle(), bone->getSpin());
            }
        }
    }
    
    if(boneRef->getParent() != -1) {
        BoneRef* parent = m_owner->getTimedBoneReference(boneRef->getParent(), time);
        if(parent != NULL) {
            Transform parentTransform = buildTransform(parent, key, time, length, looping);
            boneTransform.apply_parent_transform(parentTransform);
        }
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
    
    out << "\t\t[" << timeline.m_owner->objectCounter << "] = {" << endl;
    int keyNum = 0;
    for(vector<MainlineKey*>::const_iterator itMain = timeline.m_owner->m_mainlineKeys.begin(); itMain != timeline.m_owner->m_mainlineKeys.end(); itMain++) {
        MainlineKey* mKey = *itMain;
        ObjectRef* objectRef = mKey->findReferenceToObject(timeline.m_id);
        Object* firstResultObj;
        int firstZIndex = 0;
        Object* prevObj;
        bool loopbackFrameAlreadyWritten;
        for(vector<Object*>::const_iterator it = timeline.m_objects.begin(); it != timeline.m_objects.end(); ) {
            // For each object we have to check if it is attached to a bone. If it is, then
            // we need to recursively look up the bone's properties and calculate the values to add to the
            // object position.
            
            Object* object = *it;
            int mainlineKeyId = mKey->getId();
            if(objectRef == NULL) {
                objectRef = timeline.m_owner->findReferenceToObject(timeline.m_id, object->getId(), &mainlineKeyId);
            }
            if(object != NULL && objectRef != NULL && objectRef->getKey() == object->getId()) {
                // search the mainline for any references to this timeline and key pair
                int z = 0;
                
                Transform objectTransform(object->getX(), object->getY(), object->getAngle(), object->getScaleX(), object->getScaleY(), object->getSpin());
                Object* objectNextKey = timeline.m_owner->getNextObjectByTime(objectRef->getTimeline(), mKey->getTime());

                
                objectTransform.rotationAngle = Timeline::calculateActualRotationAngle(object->getAngle(), objectNextKey->getAngle(), object->getSpin());
                
                BoneRef* boneRef = timeline.m_owner->getTimedBoneReference(objectRef, mKey->getTime());
                if (boneRef != NULL) {
                    Transform parentTransform = timeline.buildTransform(boneRef, mainlineKeyId, mKey->getTime(), timeline.m_owner->getLength(), timeline.m_owner->getLooping());
                    objectTransform.apply_parent_transform(parentTransform);
                }
                
                if (objectRef != NULL) {
                    z = objectRef->getZIndex();
                }
                
                Object* resultObj = new Object();
                resultObj->setAngle(fmod(objectTransform.angle, 360));
                resultObj->setFile(object->getFile());
                resultObj->setFolder(object->getFolder());
                resultObj->setScaleX(objectTransform.scale_x);
                resultObj->setScaleY(objectTransform.scale_y);
                resultObj->setX(objectTransform.x);
                resultObj->setY(objectTransform.y);
                resultObj->setSpin(objectTransform.spin);                
                resultObj->setPivotX(object->getPivotX());
                resultObj->setPivotY(object->getPivotY());
                
                if(itMain == timeline.m_owner->m_mainlineKeys.begin()) {
                    firstResultObj = resultObj;
                    firstZIndex = objectRef->getZIndex();
                }
                
                if(prevObj == NULL || (*it)->getTime() == mKey->getTime() || !resultObj->equals(*prevObj)) {
                    Timeline::writeObject(mKey->getTime(), resultObj, timeline,  out, &keyNum, z, false);
                }
                
                if(mKey->getTime() == timeline.m_owner->getLength()) {
                    loopbackFrameAlreadyWritten = true;
                }

                ++it;
                prevObj = resultObj;
            } else {
                ++it;
            }
        }
        // This bit adds a "loopback" keyframe which is the same as the first frame,
        // if the last frame isn't specified as a keyframe. This makes it tween and loop smoothly like
        // in the Spriter GUI rather than "jerk" back to the first frame after the final frame.
        if(!loopbackFrameAlreadyWritten && (itMain + 1 == timeline.m_owner->m_mainlineKeys.end()) &&
           timeline.m_owner->getLooping() != false) {
            if(prevObj == NULL || !firstResultObj->equals(*prevObj)) {
              Timeline::writeObject(timeline.m_owner->getLength(), firstResultObj, timeline, out, &keyNum, firstZIndex, true);
            }
        }
    }
    out << "\t\t}";
    return out;
}

void Timeline::writeObject(int time, Object* resultObj, const Timeline& timeline, std::ostream& out, int* keyNum, int z, bool noPivotAdjust) {
    out << "\t\t\t[" << ++(*keyNum) << "] = {" << endl;
    
    out << "\t\t\t\t['angle'] = " << boost::format("%.4f") % resultObj->getAngle() << "," << endl;
    out << "\t\t\t\t['texture'] = '" << timeline.m_owner->getFileName(resultObj->getFolder(), resultObj->getFile()) << "'," << endl;
    out << "\t\t\t\t['zindex'] = " << z << "," << endl;
    out << "\t\t\t\t['scale_x'] = " << boost::format("%.4f") % resultObj->getScaleX() << "," << endl;
    out << "\t\t\t\t['scale_y'] = " << boost::format("%.4f") % resultObj->getScaleY() << "," << endl;
    out << "\t\t\t\t['time'] = " << time << "," << endl;
    
    // Adjust pivot points to default if they are different.
    float pivot_x = resultObj->getPivotX();
    float pivot_y = resultObj->getPivotY();
    
    if((pivot_x == 0.0 && pivot_y == 0.0)) {
        pivot_x = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getPivotX();
        pivot_y = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getPivotY();
    }
    
    if(!noPivotAdjust && (pivot_x != 0.0 || pivot_y != 0.0)) {
        int height = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getHeight();
        int width = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getWidth();
        pivot_x = pivot_x * width;
        pivot_y = pivot_y * height;
        Point *p = new Point(resultObj->getX() - pivot_x, resultObj->getY() + height - pivot_y);
        float angle = resultObj->getAngle() * M_PI / 180.0;
        Point adjustedPivot = Timeline::rotatePoint(resultObj->getX(), resultObj->getY(), angle, *p);
        
        resultObj->setX(adjustedPivot.x);
        resultObj->setY(adjustedPivot.y);
    }
    
    out << "\t\t\t\t['x'] = " << boost::format("%.6f") % resultObj->getX() << "," << endl;
    out << "\t\t\t\t['y'] = " << boost::format("%.6f") % resultObj->getY() << "," << endl;
    out << "\t\t\t\t['spin'] = " << resultObj->getSpin() << "," << endl;
    
    out << "\t\t\t\t['pivot_x'] = 0," << endl;
    out << "\t\t\t\t['pivot_y'] = " << timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getHeight() << endl;
    
    out << "\t\t\t}";
    
    if(time != timeline.m_owner->getLength()) {
        out << ", ";
    }
    out << endl;
}

Point Timeline::rotatePoint(float cx,float cy,float angle, Point p) {
    float s = sin(angle);
    float c = cos(angle);
    
    // translate point back to origin:
    p.x -= cx;
    p.y -= cy;
    
    // rotate point
    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;
    
    // translate point back:
    p.x = xnew + cx;
    p.y = ynew + cy;
    return p;
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

Bone* Timeline::getBoneByTime(int time) {
    if(isTypeObject())
        return NULL;
    for(vector<Bone*>::const_iterator it = m_bones.begin(); it != m_bones.end(); it++) {
        if((*it)->getTime() == time) {
            return (*it);
        } else if((*it)->getTime() > time) {
            it--;
            return (*it);
        } else if(it+1 == m_bones.end()) {
            return (*it);
        }
    }
    return NULL;
}

Bone* Timeline::getNextBoneByTime(int time) {
    if(isTypeObject())
        return NULL;
    for(vector<Bone*>::const_iterator it = m_bones.begin(); it != m_bones.end(); it++) {
        if((*it)->getTime() == time) {
            it++;
            if(it != m_bones.end()) {
                return (*it);
            } else {
                it = m_bones.begin();
                return (*it);
            }
        } else if((*it)->getTime() > time) {
            return (*it);
        } else if(it+1 == m_bones.end()) {
            it = m_bones.begin();
            return (*it);
        }
    }
    return NULL;
}

Object* Timeline::getNextObjectByTime(int time) {
    if(!isTypeObject())
        return NULL;
    for(vector<Object*>::const_iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if((*it)->getTime() == time) {
            it++;
            if(it != m_objects.end()) {
                return (*it);
            } else {
                it = m_objects.begin();
                return (*it);
            }
        } else if((*it)->getTime() > time) {
            return (*it);
        } else if(it+1 == m_objects.end()) {
            it = m_objects.begin();
            return (*it);
        }
    }
    return NULL;
}


