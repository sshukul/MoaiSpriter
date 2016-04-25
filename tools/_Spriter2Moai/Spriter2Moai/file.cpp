//
//  file.ccpp.c
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <stdio.h>
#include "File.h"

using namespace tinyxml2;

void File::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
    
    attb = a_element->FindAttribute("width");
    if(attb) {
        m_width = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("height");
    if(attb) {
        m_height = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("pivot_x");
    if(attb) {
        m_pivot_x = attb->FloatValue();
    }
    
    attb = a_element->FindAttribute("pivot_y");
    if(attb) {
        m_pivot_y = attb->FloatValue();
    }
}