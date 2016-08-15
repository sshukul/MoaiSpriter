//
//  Timeline.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__Timeline__
#define __Spriter2Moai__Timeline__

#include <string>
#include <vector>
#include "tinyxml2.h"
#include "transform.h"

using namespace std;

class Animation;
class Object;
class ObjectRef;
class MainlineKey;
class Bone;
class BoneRef;

class Timeline {
private:
    Animation* m_owner;
    int m_id;
    string m_name;
    string m_objectType;
    
    // These are attributes for custom animation curves like cubic, quadratic etc.
    string m_curve_type = "";
    float m_c1 = -1.0;
    float m_c2 = -1.0;
    float m_c3 = -1.0;
    float m_c4 = -1.0;
    
    vector<Object*> m_objects;
    vector<Bone*> m_bones;
    
    static void writeObject(int time, Object* resultObj, const Timeline& timeline, std::ostream& out, int* keyNum, int z, Object* prevObj, bool hasNext, MainlineKey* mKey);
    
    static float calculateActualRotationAngle(float startAngle, float endAngle, int spin);
    
public:
    Timeline(): m_owner(NULL), m_id(-1), m_name(""), m_objectType("object") {}
    Timeline(Animation* a_owner): m_owner(a_owner), m_id(-1), m_name(""), m_objectType("object") {}
    ~Timeline();
    
    inline Animation* getOwner() const {return m_owner;}
    inline void setOwner(Animation* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    inline const string& getObjectType() const {return m_objectType;}
    inline void setObjectType(string a_objectType) {m_objectType = a_objectType;}
    inline string getCurveType() const {return m_curve_type;}
    inline float getC1() const {return m_c1;}
    inline float getC2() const {return m_c2;}
    inline float getC3() const {return m_c3;}
    inline float getC4() const {return m_c4;}
    
    inline bool isTypeObject() const {return (m_objectType.compare("object")==0);}
    inline bool isTypePoint() const {return (m_objectType.compare("point")==0);}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, const Timeline& timeline);
    
    std::pair<Transform, bool>* buildTransform(BoneRef* objectRef, int key, int time, int length, bool looping, bool objectHasSoundlineFrame, int prevFrameTime, int nextFrameTime) const;
    
    void addObject(Object* a_object);
    void addBone(Bone* a_bone);
    
    Object* getObject(int a_index);
    Object* getObjectByTime(int time);
    Object* getNextObjectByTime(int time);
    Bone* getBone(int a_index);
    Bone* getBoneByTime(int time);
    Bone* getNextBoneByTime(int time);
    Bone* getPreviousBoneByTime(int time) const;
};

std::ostream& operator<< (std::ostream& out, const Timeline& timeline);

#endif /* defined(__Spriter2Moai__Timeline__) */
