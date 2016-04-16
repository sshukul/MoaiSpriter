//
//  Animation.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__Animation__
#define __Spriter2Moai__Animation__

#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;

class MainlineKey;
class Timeline;
class Bone;
class Object;
class Entity;
class File;
class ObjectRef;
class BoneRef;

class Animation {
private:
    Entity* m_owner;
    int m_id;
    string m_name;
    unsigned int m_length;
    bool m_looping = true;
    vector<Timeline*> m_timelines;
    
public:
    vector<MainlineKey*> m_mainlineKeys;
    Animation(): m_owner(NULL), m_id(-1), m_name(""), m_length(0), m_looping(true) {}
    Animation(Entity* a_owner): m_owner(a_owner), m_id(-1), m_name(""), m_length(0), m_looping(true) {}
    ~Animation();
        
    inline Entity* getOwner() const {return m_owner;}
    inline void setOwner(Entity* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    inline unsigned int getLength() const {return m_length;}
    inline void setLength(unsigned int a_length) {m_length = a_length;}
    inline bool getLooping() const {return m_looping;}
    inline void setLooping(bool a_looping) {m_looping = a_looping;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, const Animation& animation);
    
    void addMainlineKey(MainlineKey* a_key);
    void addTimeline(Timeline* a_timeline);
    inline const Timeline* getTimeline(int timeline_id) {return m_timelines[timeline_id];}
    
    Bone* getBone(unsigned int a_timelineIndex, unsigned int a_keyIndex);
    Bone* getBoneByTime(unsigned int a_timelineIndex, int time);
    Bone* getNextBoneByTime(unsigned int a_timelineIndex, int time);
    Object* getObject(unsigned int a_timelineIndex, unsigned int a_keyIndex);
    Object* getNextObjectByTime(unsigned int a_timelineIndex, int time);
    string getFileName(unsigned int a_folderIndex, unsigned int a_fileIndex);
    File* getFile(unsigned int a_folderIndex, unsigned int a_fileIndex);
    
    ObjectRef* findReferenceToObject(unsigned int a_timelineIndex, unsigned int a_keyIndex, int* keyId);
    BoneRef* getBoneReference(ObjectRef* a_objectRef, int a_keyId);
    BoneRef* getBoneReference(unsigned int a_boneRefId, unsigned int a_keyId);
    BoneRef* getTimedBoneReference(ObjectRef* a_objectRef, int time);
    BoneRef* getTimedBoneReference(unsigned int a_boneRefId, int time);

};

std::ostream& operator<< (std::ostream& out, const Animation& animation);

#endif /* defined(__Spriter2Moai__Animation__) */
