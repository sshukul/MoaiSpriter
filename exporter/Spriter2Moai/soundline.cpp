//
//  soundline.cpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 14/05/16.
//


#include <iostream>
#include <boost/format.hpp>
#include <math.h>
#include "soundline.h"
#include "object.h"
#include "animation.h"
#include "objectRef.h"
#include "file.h"
#include "transform.h"

using namespace std;
using namespace tinyxml2;

Soundline::~Soundline() {
}

void Soundline::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
    
    attb = a_element->FindAttribute("object_type");
    if(attb) {
        m_objectType = attb->Value();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while(child) {
        if(strcmp(child->Name(), "key") == 0) {
            if (m_objectType.compare("object") == 0) {
                Object* object = new Object();
                object->loadXML(child);
                addObject(object);
            }
        }
        child = child->NextSiblingElement();
    }
}

std::ostream& operator<< (std::ostream& out, const Soundline& soundline) {
    out << "\t\t\t['" << soundline.getName() << "'] = {" << endl;
    int keyNum = 0;
    
    for(vector<Object*>::const_iterator it = soundline.m_objects.begin(); it != soundline.m_objects.end(); it++) {
        out << "\t\t\t\t[" << ++keyNum << "] = {" << endl;
        Object* object = *(it);
        
        out << "\t\t\t\t\t['time'] = " << object->getTime() << ", " << endl;
        out << "\t\t\t\t\t['sound'] = '" << soundline.m_owner->getFileName(object->getFolder(), object->getFile()) << "'" << endl;
        out << "\t\t\t\t}";
        
        if(it+1 != soundline.m_objects.end()) {
            out << ", ";
        }
        out << endl;
    }
    out << "\t\t\t}";
    return out;
}

void Soundline::addObject(Object* a_object) {
    m_objects.push_back(a_object);
}


