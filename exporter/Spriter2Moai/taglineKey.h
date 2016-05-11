//
//  taglineKey.hpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//  Copyright © 2016 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__TaglineKey__
#define __Spriter2Moai__TaglineKey__

#include <vector>
#include "tinyxml2.h"
#include "object.h"

using namespace std;

class Tag;
class Animation;

class TaglineKey {
private:
    Animation* m_owner;
    int m_id;
    unsigned int m_time;
    
public:
    vector<Tag*> m_tags;
    
    TaglineKey(): m_owner(NULL), m_id(-1), m_time(0) {}
    TaglineKey(Animation* a_owner): m_owner(a_owner), m_id(-1), m_time(0) {}
    ~TaglineKey();
    
    inline Animation* getOwner() const {return m_owner;}
    inline void setOwner(Animation* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline unsigned int getTime() const {return m_time;}
    inline void setTime(unsigned int a_time) {m_time = a_time;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, const TaglineKey& taglineKey);
    
    void addTag(Tag* a_tag);
};

std::ostream& operator<< (std::ostream& out, const TaglineKey& taglineKey);

#endif /* taglineKey_h */
