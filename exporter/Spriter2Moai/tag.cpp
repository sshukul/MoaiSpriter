//
//  tag.cpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 07/05/16.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include "tag.h"
#include "tagList.h"
#include "spriterData.h"

using namespace std;
using namespace tinyxml2;
using namespace boost::filesystem;

void Tag::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
}

void Tag::loadXML2(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("t");
    if(attb) {
        m_id = attb->IntValue();
    }
}

std::ostream& operator<< (std::ostream& out, Tag& tag) {
    out << "\t\t\t\t\t'" << tag.getName() << "'";
    return out;
}


