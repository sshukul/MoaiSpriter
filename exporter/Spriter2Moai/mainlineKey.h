//
//  Key.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__Key__
#define __Spriter2Moai__Key__

#include <vector>
#include "tinyxml2.h"
#include "object.h"

using namespace std;

class BoneRef;
class ObjectRef;
class Bone;
class Animation;

class MainlineKey {
private:
    Animation* m_owner;
    int m_id;
    unsigned int m_time;
    int m_spin;
    
    // These are attributes for custom animation curves like cubic, quadratic etc.
    string m_curve_type = "";
    float m_c1 = -1.0;
    float m_c2 = -1.0;
    float m_c3 = -1.0;
    float m_c4 = -1.0;
    
    vector<BoneRef*> m_boneRefs;
    vector<ObjectRef*> m_objectRefs;
    
public:
    MainlineKey(): m_owner(NULL), m_id(-1), m_time(0), m_spin(1) {}
    MainlineKey(Animation* a_owner): m_owner(a_owner), m_id(-1), m_time(0), m_spin(0) {}
    ~MainlineKey();
    
    inline Animation* getOwner() const {return m_owner;}
    inline void setOwner(Animation* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline unsigned int getTime() const {return m_time;}
    inline void setTime(unsigned int a_time) {m_time = a_time;}
    inline int getSpin() const {return m_spin;}
    inline void setSpin(int a_spin) {m_spin = a_spin;}
    inline string getCurveType() const {return m_curve_type;}
    inline float getC1() const {return m_c1;}
    inline float getC2() const {return m_c2;}
    inline float getC3() const {return m_c3;}
    inline float getC4() const {return m_c4;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
//    void writeMoai(ostream& out) const;
    
    void addBoneRef(BoneRef* a_boneRef);
    void addObjectRef(ObjectRef* a_objectRef);
    
    BoneRef* getBoneReference(ObjectRef* a_objectRef);
    BoneRef* getBoneReference(unsigned int a_id);
    
    ObjectRef* findReferenceToObject(int a_timelineIndex, unsigned int a_keyIndex);
    ObjectRef* findReferenceToObject(int a_timelineIndex);
    
    MainlineKey* clone();
};

#endif /* defined(__Spriter2Moai__Key__) */
