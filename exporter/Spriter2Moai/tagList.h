//
//  tagList.hpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//


#ifndef Spriter2Moai_tagList_h
#define Spriter2Moai_tagList_h

#include <vector>
#include <string>
#include "tinyxml2.h"

using namespace std;

class Tag;
class SpriterData;

class TagList {
private:
    SpriterData* m_owner;
    vector<Tag*> m_tags;
    
public:
    TagList(): m_owner(NULL) {}
    TagList(SpriterData* a_owner): m_owner(a_owner) {}
    ~TagList();
    
    inline SpriterData* getOwner() const {return m_owner;}
    inline void setOwner(SpriterData* a_owner) {m_owner = a_owner;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);

    //friend std::ostream& operator<< (std::ostream& out, const TagList& tagList);
    
    void addTag(Tag* a_tag);
    Tag* getTag(unsigned int a_tagIndex);
};

//std::ostream& operator<< (std::ostream& out, const TagList& tagList);

#endif
