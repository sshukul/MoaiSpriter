//
//  boneRef.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include "boneRef.h"

using namespace tinyxml2;

void BoneRef::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("parent");
    if(attb) {
        m_parent = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("timeline");
    if(attb) {
        m_timeline = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("key");
    if(attb) {
        m_key = attb->IntValue();
    }
}