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
#include "taglineKey.h"
#include "tag.h"
#include "timeline.h"
#include "soundline.h"
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
    
    for (vector<TaglineKey*>::iterator it = m_taglineKeys.begin(); it != m_taglineKeys.end(); it++) {
        delete (*it);
    }
    m_taglineKeys.clear();
    
    for (vector<Timeline*>::iterator it = m_timelines.begin(); it != m_timelines.end(); it++) {
        delete (*it);
    }
    m_timelines.clear();
}

void Animation::addMainlineKey(MainlineKey* a_key) {
    m_mainlineKeys.push_back(a_key);
}

void Animation::addTaglineKey(TaglineKey* a_key) {
    m_taglineKeys.push_back(a_key);
}

void Animation::addTimeline(Timeline* a_timeline) {
    m_timelines.push_back(a_timeline);
}

void Animation::addSoundline(Soundline* a_soundline) {
    m_soundlines.push_back(a_soundline);
}

Bone* Animation::getBone(unsigned int a_timelineIndex, unsigned int a_keyIndex) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex] == NULL)
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("bone") == 0) {
        return m_timelines[a_timelineIndex]->getBone(a_keyIndex);
    }
    return NULL;
}

Bone* Animation::getBoneByTime(unsigned int a_timelineIndex, int time) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex] == NULL)
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("bone") == 0) {
        return m_timelines[a_timelineIndex]->getBoneByTime(time);
    }
    return NULL;
}

Bone* Animation::getNextBoneByTime(unsigned int a_timelineIndex, int time) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex] == NULL)
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("bone") == 0) {
        return m_timelines[a_timelineIndex]->getNextBoneByTime(time);
    }
    return NULL;
}

Object* Animation::getObject(unsigned int a_timelineIndex, unsigned int a_keyIndex) {    
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex]->getObjectType().compare("object") == 0 ||
       m_timelines[a_timelineIndex]->getObjectType().compare("point") == 0) {
        return m_timelines[a_timelineIndex]->getObject(a_keyIndex);
    }
    return NULL;
}

Object* Animation::getObjectByTime(unsigned int a_timelineIndex, int time) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex] == NULL)
        return NULL;
    
    return m_timelines[a_timelineIndex]->getObjectByTime(time);
}

Object* Animation::getNextObjectByTime(unsigned int a_timelineIndex, int time) {
    if(a_timelineIndex >= m_timelines.size())
        return NULL;
    
    if(m_timelines[a_timelineIndex] == NULL)
        return NULL;
    
    return m_timelines[a_timelineIndex]->getNextObjectByTime(time);
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

BoneRef* Animation::getTimedBoneReference(ObjectRef* a_objectRef, int time) {
    for(vector<MainlineKey*>::iterator it = m_mainlineKeys.begin(); it != m_mainlineKeys.end(); ++it) {
        if((*it)->getTime() == time) {
            return (*it)->getBoneReference(a_objectRef);
        }
    }
    return NULL;
}

BoneRef* Animation::getBoneReference(unsigned int a_boneRefId, unsigned int a_keyId) {
    if(a_keyId >= m_mainlineKeys.size())
        return NULL;
    return m_mainlineKeys[a_keyId]->getBoneReference(a_boneRefId);
}

BoneRef* Animation::getTimedBoneReference(unsigned int a_boneRefId, int time) {
    for(vector<MainlineKey*>::iterator it = m_mainlineKeys.begin(); it != m_mainlineKeys.end(); ++it) {
        if((*it)->getTime() == time) {
            return (*it)->getBoneReference(a_boneRefId);
        }
    }
    return NULL;
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
        else if(strcmp(child->Name(), "soundline") == 0) {
            Soundline* soundline = new Soundline(this);
            soundline->loadXML(child);
            addSoundline(soundline);
        }
        else if(strcmp(child->Name(), "meta") == 0) {
            const tinyxml2::XMLElement* metaChild = child->FirstChildElement();
            while (metaChild) {
                if(strcmp(metaChild->Name(), "tagline") == 0) {
                    const tinyxml2::XMLElement* taglineChild = metaChild->FirstChildElement();
                    while (taglineChild) {
                        if(strcmp(taglineChild->Name(), "key") == 0) {
                            TaglineKey* key = new TaglineKey(this);
                            key->loadXML(taglineChild);
                            addTaglineKey(key);
                        }
                        taglineChild = taglineChild->NextSiblingElement();
                    }
                }
                metaChild = metaChild->NextSiblingElement();
            }
        }
        child = child->NextSiblingElement();
    }
}

std::ostream& operator<< (std::ostream& out, Animation& animation) {
    cout << "Converting animation " << animation.m_name;
    out << "\t['" << animation.m_name << "'] = {" << endl;
    out << "\t\t['objects'] = {" << endl;
    
    // count how many object timelines we have
    int objectCnt = 0;
    for(vector<Timeline*>::const_iterator it = animation.m_timelines.begin(); it != animation.m_timelines.end(); ++it) {
        if((*it)->isTypeObject() || (*it)->isTypePoint())
            objectCnt++;
    }
    
    animation.objectCounter = 0;
    for(vector<Timeline*>::const_iterator it = animation.m_timelines.begin(); it != animation.m_timelines.end(); ++it) {
        if((*it)->isTypeObject() || (*it)->isTypePoint()) {
            ++animation.objectCounter;
            out << *(*it);
        
            if(animation.objectCounter < objectCnt)
                out << ",";
            out << endl;
        }
    }
    out << "\t\t}";
    
    // Add sounds if any
    if(animation.m_soundlines.size() > 0) {
        out << "," << endl << "\t\t['sounds'] = {" << endl;
        for(vector<Soundline*>::const_iterator it = animation.m_soundlines.begin(); it != animation.m_soundlines.end(); ++it) {
            out << *(*it);
            if(it+1 != animation.m_soundlines.end()) {
                out << ",";
            }
        }
        out << endl << "\t\t}";
    }
    
    // Add tags to meta section
    if(animation.m_taglineKeys.size() > 0) {
        out << "," << endl << "\t\t['meta'] = {" << endl;
        out << "\t\t\t['tags'] = {" << endl;
        for(vector<TaglineKey*>::const_iterator it = animation.m_taglineKeys.begin(); it != animation.m_taglineKeys.end(); ++it) {
            out << *(*it);
            if(it+1 != animation.m_taglineKeys.end()) {
                out << ",";
            }
            out << endl;
        }
        out << "\t\t\t}";
        out << endl << "\t\t}";
    }

    out << endl << "\t}";
    cout << " ... Done.\n";
    return out;
}


