//
//  taglineKey.cpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//  Copyright © 2016 Samuel Cartwright. All rights reserved.
//

#include <iostream>
#include "taglineKey.h"
#include "tag.h"
#include "entity.h"
#include "animation.h"
#include "spriterData.h"

using namespace std;
using namespace tinyxml2;

TaglineKey::~TaglineKey() {
    for(vector<Tag*>::iterator it = m_tags.begin(); it != m_tags.end(); it++) {
        delete (*it);
    }
    m_tags.empty();
}

void TaglineKey::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("time");
    if(attb) {
        m_time = attb->IntValue();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while(child) {
        if(strcmp(child->Name(), "tag") == 0) {
            Tag* tag = new Tag();
            tag->loadXML2(child);
            addTag(tag);
        }
        child = child->NextSiblingElement();
    }
}

void TaglineKey::addTag(Tag* a_tag) {
    m_tags.push_back(a_tag);
}

std::ostream& operator<< (std::ostream& out, const TaglineKey& taglineKey) {
    out << "\t\t\t\t[" << taglineKey.getTime() << "] = {" << endl;
    SpriterData* spriterData = taglineKey.getOwner()->getOwner()->getOwner();
    for(vector<Tag*>::const_iterator itTags = taglineKey.m_tags.begin(); itTags != taglineKey.m_tags.end(); ++itTags) {
        spriterData->populateTagNameFromTagList(*itTags);
        out << *(*itTags);
        if(itTags+1 != taglineKey.m_tags.end()) {
            out << ",";
        }
        out << endl;
    }
    out << "\t\t\t\t}";
    return out;
}
