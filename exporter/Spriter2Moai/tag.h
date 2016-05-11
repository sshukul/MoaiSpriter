//
//  tag.hpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//

#ifndef Spriter2Moai_tag_h
#define Spriter2Moai_tag_h

#include <string>
#include "tinyxml2.h"

using namespace std;

class TagList;

class Tag {
private:
    TagList* m_owner;
    int m_id;
    string m_name;
    
public:
    Tag(): m_id(-1), m_name("")  {}
    Tag(TagList* a_owner): m_owner(a_owner), m_id(-1), m_name("") {}
    
    ~Tag() {}
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    
    friend std::ostream& operator<< (std::ostream& out, Tag& tag);
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    void loadXML2(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, Tag& tag);
};

std::ostream& operator<< (std::ostream& out, Tag& tag);

#endif
