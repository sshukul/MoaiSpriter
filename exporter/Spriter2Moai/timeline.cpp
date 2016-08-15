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
#include "soundline.h"
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
        if(strcmp(child->Name(), "key") == 0) {
            if (m_objectType.compare("object") == 0 || m_objectType.compare("point") == 0) {
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

std::pair<Transform, bool>* Timeline::buildTransform(BoneRef* boneRef, int key, int time, int length, bool looping, bool objectHasSoundlineFrame, int prevFrameTime, int nextFrameTime) const {
    Bone* bone = m_owner->getBone(boneRef->getTimeline(), boneRef->getKey());
    Transform boneTransform(0, 0, 0, 0, 0, 0, 0);
    bool isTimelineKeyframe = false;
    if(bone != NULL) {
        if(time == bone->getTime()) {
            isTimelineKeyframe = true;
        }
        boneTransform = Transform(bone->getX(), bone->getY(), bone->getAngle(), bone->getScaleX(), bone->getScaleY(), bone->getSpin(), 1.0);
        Transform boneNextKeyTransform(bone->getX(), bone->getY(), bone->getAngle(), bone->getScaleX(), bone->getScaleY(), bone->getSpin(), 1.0);
        
        //Bone* boneNextKey = m_owner->getBone(boneRef->getTimeline(), key);
        Bone* boneNextKey = m_owner->getNextBoneByTime(boneRef->getTimeline(), time);
        Bone* prevBone = m_owner->getPreviousBoneByTime(boneRef->getTimeline(), time);
        int nextBoneTime = boneNextKey->getTime();
        if(nextBoneTime == 0 && looping) {
            nextBoneTime = length;
        }
        
        Transform nextKeyTransform(boneNextKey->getX(), boneNextKey->getY(), boneNextKey->getAngle(), boneNextKey->getScaleX(), boneNextKey->getScaleY(), boneNextKey->getSpin(), 1.0);
         
        if(objectHasSoundlineFrame && time != bone->getTime() && boneNextKey != NULL && boneNextKey->getTime() != time) {
            if(!(looping == false && boneNextKey->getTime() == 0)) {
                float averagingFactor = ((float)time - (float)prevFrameTime) / (boneNextKey->getTime() - (float)prevFrameTime);
                boneTransform = *new Transform(prevBone->getX(), prevBone->getY(), prevBone->getAngle(), prevBone->getScaleX(), prevBone->getScaleY(), prevBone->getSpin(), 1.0);
                boneTransform.lerp(nextKeyTransform, averagingFactor, prevBone->getSpin());
                boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, nextKeyTransform.angle, bone->getSpin());
            }
        } else if(time != bone->getTime() && boneNextKey != NULL && boneNextKey->getTime() != bone->getTime()) {
            if(!(looping == false && boneNextKey->getTime() == 0)) {
                float averagingFactor = ((float)time - (float)bone->getTime()) / ((float)nextBoneTime - (float)bone->getTime());
                boneTransform.lerp(nextKeyTransform, averagingFactor, bone->getSpin());
                boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, nextKeyTransform.angle, bone->getSpin());
            }
        } else if (time < bone->getTime() && boneNextKey != NULL && boneNextKey->getTime() != time && time != prevBone->getTime()) {
            if(!(looping == false && nextBoneTime == 0)) {
                if(nextBoneTime != prevBone->getTime()) {
                    float averagingFactor = ((float)time - (float)prevBone->getTime()) / (nextBoneTime - (float)prevBone->getTime());
                    boneTransform = *new Transform(prevBone->getX(), prevBone->getY(), prevBone->getAngle(), prevBone->getScaleX(), prevBone->getScaleY(), prevBone->getSpin(), 1.0);
                    boneTransform.lerp(nextKeyTransform, averagingFactor, prevBone->getSpin());
                    boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, nextKeyTransform.angle, bone->getSpin());
                }
            }
        } else if(boneNextKey != NULL && nextBoneTime != bone->getTime()) {
            boneTransform.rotationAngle = Timeline::calculateActualRotationAngle(boneTransform.angle, boneNextKey->getAngle(), bone->getSpin());
        }
        
        if(nextBoneTime != nextFrameTime && nextFrameTime != time) {
            float averagingFactor = ((float)nextFrameTime - (float)time) / ((float)nextBoneTime - (float)time);
            boneTransform.rotationAngle = boneTransform.rotationAngle * averagingFactor;
        }
    }
    
    if(boneRef->getParent() != -1) {
        BoneRef* parent = m_owner->getTimedBoneReference(boneRef->getParent(), time);
        if(parent == NULL) {
            parent = m_owner->getBoneReference(boneRef->getParent(), key);
        }
        if(parent != NULL) {
            pair<Transform, bool>* result = buildTransform(parent, key, time, length, looping, objectHasSoundlineFrame, prevFrameTime, nextFrameTime);
            boneTransform.apply_parent_transform(result->first);
            isTimelineKeyframe = isTimelineKeyframe || result->second;
        }
    }
    
    return new std::pair<Transform, bool>(boneTransform, isTimelineKeyframe);
}

// this is the trickest part in the program, because we need to output the objects in each timeline (each timeline
// describes one object), but we need to keep refering back to the mainline to see if the object is attached to any
// bones, and if so, calculate how this affects the final position of the object
// Note that we're only interested in exporting objects - not bones
std::ostream& operator<< (std::ostream& out, const Timeline& timeline) {
    
    if(!timeline.isTypeObject() && !timeline.isTypePoint())
        return out;
    
    out << "\t\t\t[" << timeline.m_owner->objectCounter << "] = {" << endl;
    int keyNum = 0;
    Object* firstResultObj = NULL;
    int firstZIndex = 0;
    Object* prevObj = NULL;
    Object* prevResultObj = NULL;
    bool loopbackFrameAlreadyWritten = false;
    bool objectHasNonMainlineFrame = false;
    bool objectHasSoundlineFrame = false;
    int prevObjTime = 0;
    int prevFrameTime = 0;
    vector<Object*>::const_iterator itObj = timeline.m_objects.begin();
    vector<Soundline*>::const_iterator itSoundlines = timeline.m_owner->m_soundlines.begin();
    vector<Object*>::const_iterator itSounds;
    
    // We iterate through the soundline if any and add it's points as keyframes too,
    // to allow attaching a listener on keyframes in Moai and playing the required sounds
    Soundline* soundline = NULL;
    if(itSoundlines != timeline.m_owner->m_soundlines.end()) {
        soundline = *itSoundlines;
        itSounds = soundline->m_objects.begin();
    }
    
    for(vector<MainlineKey*>::const_iterator itMain = timeline.m_owner->m_mainlineKeys.begin(); itMain != timeline.m_owner->m_mainlineKeys.end() || itObj != timeline.m_objects.end() || (soundline != NULL && itSounds != soundline->m_objects.end()); ) {
        bool isTimelineKeyframe = false;
        unsigned int frameTime = 0;
        
        MainlineKey* mKey = *itMain;
        Object* object = *itObj;
        Object* sound = NULL;
        
        if(soundline != NULL && itSounds != soundline->m_objects.end()) {
            sound = *itSounds;
        }
        
        int mainlineKeyTime = 0;
        int objectTime = 0;
        int soundlineTime = 0;
        bool skipFrame = false;
        if(mKey == NULL || itMain == timeline.m_owner->m_mainlineKeys.end()) {
            mainlineKeyTime = timeline.m_owner->getLength();
            mKey = *(itMain - 1);
        } else {
            mainlineKeyTime = mKey->getTime();
        }
        
        if(object == NULL || itObj == timeline.m_objects.end()) {
            objectTime = timeline.m_owner->getLength();
        } else {
            objectTime = object->getTime();
        }
        
        if(sound == NULL || soundline == NULL || itSounds == soundline->m_objects.end()) {
            soundlineTime = timeline.m_owner->getLength();
        } else {
            soundlineTime = sound->getTime();
        }
        
        if(objectTime < mainlineKeyTime) {
            if(soundlineTime < objectTime) {
                frameTime = soundlineTime;
                objectHasSoundlineFrame = true;
            } else {
                frameTime = objectTime;
            }
            objectHasNonMainlineFrame = true;
        } else if(soundlineTime < mainlineKeyTime) {
            frameTime = soundlineTime;
            objectHasSoundlineFrame = true;
        } else {
            frameTime = mainlineKeyTime;
        }
        
        if(frameTime != soundlineTime) {
            sound = NULL;
        }

        int mainlineKeyId = mKey->getId();
        ObjectRef* objectRef = mKey->findReferenceToObject(timeline.m_id);
        
        if(objectRef == NULL && object != NULL && itObj != timeline.m_objects.end()) {
            objectRef = timeline.m_owner->findReferenceToObject(timeline.m_id, object->getId(), &mainlineKeyId);
        }
        if((object == NULL || itObj == timeline.m_objects.end()) && objectRef != NULL) {
            object = timeline.m_owner->getObject(objectRef->getTimeline(), objectRef->getKey());
        }

        if(objectTime < prevObjTime ) {
            skipFrame = true;
        }
        // For each object we have to check if it is attached to a bone. If it is, then
        // we need to recursively look up the bone's properties and calculate the values to add to the
        // object position.
    
        if(objectHasNonMainlineFrame && object != NULL) {
            mainlineKeyId = object->getId();
        }
        
        if(frameTime == objectTime) {
            isTimelineKeyframe = true;
        }
    
        if(object != NULL && objectRef != NULL && !skipFrame) {
            // search the mainline for any references to this timeline and key pair
            int z = 0;
            
            Transform objectTransform(object->getX(), object->getY(), object->getAngle(), object->getScaleX(), object->getScaleY(), object->getSpin(), object->getAlpha());
            Transform objectNextKeyTransform(object->getX(), object->getY(), object->getAngle(), object->getScaleX(), object->getScaleY(), object->getSpin(), object->getAlpha());
            
            //Object* objectNextKey = timeline.m_owner->getNextObjectByTime(objectRef->getTimeline(), frameTime);
        
            Object* nextObject = timeline.m_owner->getObject(timeline.getId(), object->getId()+1);
            
            if(nextObject != NULL && nextObject->getTime() == object->getTime() && nextObject->getId() != object->getId()) {
                skipFrame = true;
            }
            
            int nextSoundlineTime = soundlineTime;
            if(nextSoundlineTime == frameTime) {
                if(soundline != NULL && itSounds != soundline->m_objects.end() && (itSounds + 1) != soundline->m_objects.end()) {
                    nextSoundlineTime = (*(itSounds + 1))->getTime();
                }
            }
            int nextMainlineKeyTime = mainlineKeyTime;
            if(mainlineKeyTime == frameTime) {
                if(itMain != timeline.m_owner->m_mainlineKeys.end() && (itMain + 1) != timeline.m_owner->m_mainlineKeys.end()) {
                    nextMainlineKeyTime = (*(itMain + 1))->getTime();
                }
            }
            int nextAltKeyTime = min(nextSoundlineTime, nextMainlineKeyTime);
            if(nextAltKeyTime == 0) {
                nextAltKeyTime = timeline.m_owner->getLength();
            }
            int nextFrameTime = nextAltKeyTime;
            if(nextObject != NULL) {
                nextFrameTime = min(nextAltKeyTime, (int) nextObject->getTime());
            }
            Object* objectNextKey = object;
            if (objectTime < frameTime) {
                objectNextKey = timeline.m_owner->getObject(timeline.getId(), object->getId()+1);
            }
            
            if(objectHasSoundlineFrame && frameTime != object->getTime() && objectNextKey != NULL && objectNextKey->getTime() != frameTime) {
                if(!(timeline.m_owner->getLooping() == false && objectNextKey->getTime() == 0)) {
                    float averagingFactor = ((float)frameTime - (float)prevFrameTime) / (objectNextKey->getTime() - (float)prevFrameTime);
                    objectTransform = *new Transform(prevObj->getX(), prevObj->getY(), prevObj->getAngle(), prevObj->getScaleX(), prevObj->getScaleY(), prevObj->getSpin(), prevObj->getAlpha());
                    Transform nextKeyTransform(objectNextKey->getX(), objectNextKey->getY(), objectNextKey->getAngle(), objectNextKey->getScaleX(), objectNextKey->getScaleY(), objectNextKey->getSpin(), objectNextKey->getAlpha());
                    objectTransform.lerp(nextKeyTransform, averagingFactor, prevObj->getSpin());
                }
            } else if (frameTime < object->getTime() && objectNextKey != NULL && objectNextKey->getTime() != frameTime && frameTime != prevObjTime) {
                float nextFrameTime = objectNextKey->getTime();
                if(!(timeline.m_owner->getLooping() == false && nextFrameTime == 0)) {
                    if(nextFrameTime == 0) {
                        nextFrameTime = timeline.m_owner->getLength();
                    }
                    if(nextFrameTime != prevObjTime) {
                        float averagingFactor = ((float)frameTime - (float)prevObjTime) / (nextFrameTime - (float)prevObjTime);
                        objectTransform = *new Transform(prevObj->getX(), prevObj->getY(), prevObj->getAngle(), prevObj->getScaleX(), prevObj->getScaleY(), prevObj->getSpin(), prevObj->getAlpha());
                        Transform nextKeyTransform(objectNextKey->getX(), objectNextKey->getY(), objectNextKey->getAngle(), objectNextKey->getScaleX(), objectNextKey->getScaleY(), objectNextKey->getSpin(), objectNextKey->getAlpha());
                        objectTransform.lerp(nextKeyTransform, averagingFactor, prevObj->getSpin());
                    }
                }
            }
            
            if(nextObject != NULL && object->getTime() != nextObject->getTime()) {
                int nextKeyTime = nextObject->getTime();
                if(nextObject->getTime() == 0) {
                    nextKeyTime = timeline.m_owner->getLength();
                }
                
                
                // Find the object angle at the next keyframe, based on lerping if needed. Use it
                // to calculate the rotation angle and spin for the object.
                if(nextKeyTime != nextAltKeyTime) {
                    bool skipLerp = false;
                    if(nextObject->getTime() == 0 && timeline.m_owner->getLooping() == false) {
                        skipLerp = true;
                    }
                    // If there is no next object keyframe and we are not looping, then there is no
                    // rotation for this object. Otherwise calculate linear interpolation to find
                    // the rotation angle for the next keyframe.
                    if(!skipLerp) {
                        float averagingFactor = ((float)nextAltKeyTime - (float) object->getTime()) / ((float) nextKeyTime - (float) object->getTime());
                        Transform nextKeyTransform(nextObject->getX(), nextObject->getY(), nextObject->getAngle(), nextObject->getScaleX(), nextObject->getScaleY(), nextObject->getSpin(), nextObject->getAlpha());
                        objectNextKeyTransform.lerp(nextKeyTransform, averagingFactor, object->getSpin());
                        objectTransform.rotationAngle = Timeline::calculateActualRotationAngle(objectTransform.angle, objectNextKeyTransform.angle, object->getSpin());
                    } else {
                        objectTransform.rotationAngle = 0;
                        objectTransform.spin = 0;
                    }
                } else {
                    objectTransform.rotationAngle = Timeline::calculateActualRotationAngle(objectTransform.angle, nextObject->getAngle(), object->getSpin());
                }
            }
            
            BoneRef* boneRef = timeline.m_owner->getTimedBoneReference(objectRef, frameTime);
            if(boneRef == NULL) {
                boneRef = timeline.m_owner->getBoneReference(objectRef, mainlineKeyId);
            }
            if (boneRef != NULL) {
                pair<Transform, bool>* result = timeline.buildTransform(boneRef, mainlineKeyId, frameTime, timeline.m_owner->getLength(), timeline.m_owner->getLooping(), objectHasSoundlineFrame, prevFrameTime, nextFrameTime);
                objectTransform.apply_parent_transform(result->first);
                isTimelineKeyframe = isTimelineKeyframe || result->second;
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
            resultObj->setAlpha(objectTransform.alpha);
            
            if(itMain == timeline.m_owner->m_mainlineKeys.begin()) {
                firstResultObj = resultObj;
                firstZIndex = objectRef->getZIndex();
            }
            
            bool hasNext = false;
            if(((*itObj) != NULL && itObj != timeline.m_objects.end()) || ((*itMain) != NULL && itMain != timeline.m_owner->m_mainlineKeys.end()) || (soundline != NULL && (*itSounds) != NULL && itSounds != soundline->m_objects.end())) {
                hasNext = true;
            }

            if(isTimelineKeyframe || objectHasSoundlineFrame) {
                if(!skipFrame && ((frameTime == timeline.m_owner->getLength() && timeline.m_owner->getLooping() != false) || prevResultObj == NULL || !resultObj->equals(*prevResultObj) || (objectHasSoundlineFrame && (soundlineTime != prevFrameTime)))) {
                    Timeline::writeObject(frameTime, resultObj, timeline,  out, &keyNum, z, prevResultObj, hasNext, mKey);
                    if(frameTime == timeline.m_owner->getLength()) {
                        loopbackFrameAlreadyWritten = true;
                    }
                }
            }
            if(mainlineKeyTime < objectTime || soundlineTime < objectTime) {
                if(prevResultObj != NULL) {
                    resultObj->setPivotX(prevResultObj->getPivotX());
                    resultObj->setPivotY(prevResultObj->getPivotY());
                }
            }
            prevResultObj = resultObj;
            
        }
        
        if(objectHasNonMainlineFrame) {
            objectHasNonMainlineFrame = false;
        }
        if(objectHasSoundlineFrame) {
            objectHasSoundlineFrame = false;
        }
        
        if(objectTime < mainlineKeyTime && objectTime < soundlineTime) {
            prevObj = *itObj;
            itObj++;
        } else if(mainlineKeyTime < objectTime && mainlineKeyTime < soundlineTime) {
            itMain++;
        } else if(soundlineTime < objectTime && soundlineTime < mainlineKeyTime) {
            itSounds++;
        } else {
            if((*itObj) != NULL && itObj != timeline.m_objects.end()) {
                prevObj = *itObj;
                itObj++;
            }
            if((*itMain) != NULL && itMain != timeline.m_owner->m_mainlineKeys.end()) {
                itMain++;
            }
            if(soundline != NULL && (*itSounds) != NULL && itSounds != soundline->m_objects.end()) {
                itSounds++;
            }
        }
        
        // This bit adds a "loopback" keyframe which is the same as the first frame,
        // if the last frame isn't specified as a keyframe and looping is enabled.
        // This makes it tween and loop smoothly like in the Spriter GUI rather than "jerk" back to the first frame after the final frame.
        if(!loopbackFrameAlreadyWritten && firstResultObj != NULL && timeline.m_owner->getLooping() != false && itMain == timeline.m_owner->m_mainlineKeys.end() && itObj == timeline.m_objects.end() && (soundline == NULL || ((*itSounds) != NULL && itSounds == soundline->m_objects.end()))) {
            //if(prevResultObj == NULL || !firstResultObj->equals(*prevResultObj)) {
                Timeline::writeObject(timeline.m_owner->getLength(), firstResultObj, timeline, out, &keyNum, firstZIndex, prevResultObj, false, mKey);
            //}
        }
        
        prevObjTime = objectTime;
        prevFrameTime = frameTime;
    }
    out << "\t\t\t}";
    return out;
}

void Timeline::writeObject(int time, Object* resultObj, const Timeline& timeline, std::ostream& out, int* keyNum, int z, Object* prevResultObj, bool hasNext, MainlineKey* mKey) {
    out << "\t\t\t\t[" << ++(*keyNum) << "] = {" << endl;
    
    out << "\t\t\t\t\t['angle'] = " << boost::format("%.4f") % resultObj->getAngle() << "," << endl;
    if(resultObj->getAlpha() != 1.0) {
        out << "\t\t\t\t\t['alpha'] = " << boost::format("%.4f") % resultObj->getAlpha() << "," << endl;
    }
    
    if(timeline.isTypePoint()) {
        out << "\t\t\t\t\t['type'] = 'point'," << endl;
    }
    out << "\t\t\t\t\t['name'] = '" << timeline.getName() << "'," << endl;
    out << "\t\t\t\t\t['texture'] = '" << timeline.m_owner->getFileName(resultObj->getFolder(), resultObj->getFile()) << "'," << endl;
    out << "\t\t\t\t\t['zindex'] = " << z << "," << endl;
    out << "\t\t\t\t\t['scale_x'] = " << boost::format("%.4f") % resultObj->getScaleX() << "," << endl;
    out << "\t\t\t\t\t['scale_y'] = " << boost::format("%.4f") % resultObj->getScaleY() << "," << endl;
    if(mKey != NULL && mKey->getCurveType() != "") {
        out << "\t\t\t\t\t['curve_type'] = '" << mKey->getCurveType() << "'," << endl;
    }
    if(mKey != NULL && mKey->getC1() >= 0.0) {
        out << "\t\t\t\t\t['c1'] = " << mKey->getC1() << "," << endl;
    }
    if(mKey != NULL && mKey->getC2() >= 0.0) {
        out << "\t\t\t\t\t['c2'] = " << mKey->getC2() << "," << endl;
    }
    if(mKey != NULL && mKey->getC3() >= 0.0) {
        out << "\t\t\t\t\t['c3'] = " << mKey->getC3() << "," << endl;
    }
    if(mKey != NULL && mKey->getC4() >= 0.0) {
        out << "\t\t\t\t\t['c4'] = " << mKey->getC4() << "," << endl;
    }
    out << "\t\t\t\t\t['time'] = " << time << "," << endl;
    
    // Adjust pivot points to default if they are different.
    float pivot_x = resultObj->getPivotX();
    float pivot_y = resultObj->getPivotY();
    
    if((pivot_x == 0.0 && pivot_y == 0.0)) {
        if(prevResultObj != NULL && (prevResultObj->getPivotX() != 0.0 || prevResultObj->getPivotY() != 0.0)) {
            pivot_x = prevResultObj->getPivotX();
            pivot_y = prevResultObj->getPivotY();
        } else if (resultObj->getFolder() != -1 && resultObj->getFile() != -1) {
            pivot_x = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getPivotX();
            pivot_y = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getPivotY();
        }
    }
    
    if((pivot_x != 0.0 || pivot_y != 0.0)) {
        int height = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getHeight();
        int width = timeline.m_owner->getFile(resultObj->getFolder(), resultObj->getFile())->getWidth();
        pivot_x = pivot_x * width;
        pivot_y = pivot_y * height;
    }
    
    out << "\t\t\t\t\t['x'] = " << boost::format("%.6f") % resultObj->getX() << "," << endl;
    out << "\t\t\t\t\t['y'] = " << boost::format("%.6f") % resultObj->getY() << "," << endl;
    out << "\t\t\t\t\t['spin'] = " << resultObj->getSpin() << "," << endl;
    
    out << "\t\t\t\t\t['pivot_x'] = " << pivot_x << "," << endl;
    out << "\t\t\t\t\t['pivot_y'] = " << pivot_y << endl;
    
    out << "\t\t\t\t}";
    
    if(hasNext) {
        out << ", ";
    }
    out << endl;
}

void Timeline::addObject(Object* a_object) {
    if(!isTypeObject() && !isTypePoint())
        return;
    m_objects.push_back(a_object);
}

void Timeline::addBone(Bone* a_bone) {
    if(isTypeObject() || isTypePoint())
        return;
    m_bones.push_back(a_bone);
}

Object* Timeline::getObject(int a_index) {
    if(a_index >= m_objects.size())
        return NULL;
    if(!isTypeObject() && !isTypePoint())
        return NULL;
    return m_objects[a_index];
}

Bone* Timeline::getBone(int a_index) {
    if(a_index >= m_bones.size())
        return NULL;
    if(isTypeObject() || isTypePoint())
        return NULL;
    return m_bones[a_index];
}

Bone* Timeline::getBoneByTime(int time) {
    if(isTypeObject() || isTypePoint())
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
    if(isTypeObject() || isTypePoint())
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

Bone* Timeline::getPreviousBoneByTime(int time) const {
    if(isTypeObject() || isTypePoint())
        return NULL;
    for(vector<Bone*>::const_iterator it = m_bones.begin(); it != m_bones.end(); it++) {
        if((*it)->getTime() >= time) {
            it--;
            if(it != m_bones.begin()) {
                return (*it);
            } else {
                it = m_bones.end();
                return (*it);
            }
        } else if(it+1 == m_bones.end()) {
            return (*it--);
        }
    }
    return NULL;
}

Object* Timeline::getObjectByTime(int time) {
    if(!isTypeObject() && !isTypePoint())
        return NULL;
    for(vector<Object*>::const_iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if((*it)->getTime() == time) {
            return (*it);
        } else if((*it)->getTime() > time) {
            it--;
            return (*it);
        } else if(it+1 == m_objects.end()) {
            return (*it);
        }
    }
    return NULL;
}

Object* Timeline::getNextObjectByTime(int time) {
    if(!isTypeObject() && !isTypePoint())
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


