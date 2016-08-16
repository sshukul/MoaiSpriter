//
//  bone.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include "bone.h"

using namespace tinyxml2;

void Bone::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("time");
    if(attb) {
        m_time = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("spin");
    if(attb) {
        m_spin = attb->IntValue();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    if(strcmp(child->Name(), "bone") == 0) {
        attb = child->FindAttribute("x");
        if(attb) {
            m_x = attb->FloatValue();
        }
        
        attb = child->FindAttribute("y");
        if(attb) {
            m_y = attb->FloatValue();     // invert y
        }
        
        attb = child->FindAttribute("angle");
        if(attb) {
            m_angle = attb->FloatValue();      // invert angle
        }
        
        attb = child->FindAttribute("scale_x");
        if(attb) {
            m_scaleX = attb->FloatValue();
        }
        
        attb = child->FindAttribute("scale_y");
        if(attb) {
            m_scaleY = attb->FloatValue();
        }
        
        attb = a_element->FindAttribute("curve_type");
        if(attb) {
            m_curve_type = attb->Value();
        }
        
        attb = a_element->FindAttribute("c1");
        if(attb) {
            m_c1 = attb->FloatValue();
        }
        
        attb = a_element->FindAttribute("c2");
        if(attb) {
            m_c2 = attb->FloatValue();
        }
        
        attb = a_element->FindAttribute("c3");
        if(attb) {
            m_c3 = attb->FloatValue();
        }
        
        attb = a_element->FindAttribute("c4");
        if(attb) {
            m_c4 = attb->FloatValue();
        }
    }
}