//
//  tagList.cpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//

#include "tagList.h"

#include <stdio.h>
#include <iostream>
#include "tagList.h"
#include "tag.h"
#include "spriterData.h"

using namespace std;
using namespace tinyxml2;

TagList::~TagList() {
    for (vector<Tag*>::iterator it = m_tags.begin(); it != m_tags.end(); it++) {
        delete (*it);
    }
    m_tags.clear();
}

void TagList::loadXML(const tinyxml2::XMLElement* a_element) {
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while (child) {
        if(strcmp(child->Name(), "i") == 0) {
            Tag* tag = new Tag(this);
            tag->loadXML(child);
            addTag(tag);
        }
        child = child->NextSiblingElement();
    }
}

void TagList::addTag(Tag* a_tag) {
    m_tags.push_back(a_tag);
}

/* std::ostream& operator<< (std::ostream& out, const TagList& tagList){
    size_t totalTags = tagList.m_tags.size();
    int tagCounter = 0;
    if(tagList.m_tags.size() >= 1) {
        for(vector<Tag*>::const_iterator it = tagList.m_tags.begin(); it != tagList.m_tags.end(); it++) {
            tagCounter++;
            
            out << *(*it);
            
            if(tagCounter < totalTags)
                out << "," << endl;
        }
    }
    return out;
} */

Tag* TagList::getTag(unsigned int a_tagIndex) {
    if(a_tagIndex >= m_tags.size())
        return NULL;
    
    return m_tags[a_tagIndex];
}


