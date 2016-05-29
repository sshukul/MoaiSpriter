//
//  soundline.h
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 14/05/16.
//

#ifndef __Spriter2Moai__Soundline__
#define __Spriter2Moai__Soundline__

#include <string>
#include <vector>
#include "tinyxml2.h"
#include "transform.h"

using namespace std;

class Animation;
class Object;
class ObjectRef;

class Soundline {
private:
    Animation* m_owner;
    int m_id;
    string m_name;
    string m_objectType;
    
public:
    vector<Object*> m_objects;
    Soundline(): m_owner(NULL), m_id(-1), m_name(""), m_objectType("object") {}
    Soundline(Animation* a_owner): m_owner(a_owner), m_id(-1), m_name(""), m_objectType("object") {}
    ~Soundline();
    
    inline Animation* getOwner() const {return m_owner;}
    inline void setOwner(Animation* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    inline const string& getObjectType() const {return m_objectType;}
    inline void setObjectType(string a_objectType) {m_objectType = a_objectType;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, const Soundline& soundline);
    
    void addObject(Object* a_object);
};

std::ostream& operator<< (std::ostream& out, const Soundline& soundline);

#endif /* defined(__Spriter2Moai__Soundline__) */
