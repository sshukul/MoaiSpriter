//
//  entity.c
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "entity.h"
#include "animation.h"
#include "spriterData.h"

using namespace std;
using namespace tinyxml2;

Entity::~Entity() {
    for (vector<Animation*>::iterator it = m_animations.begin(); it != m_animations.end(); it++) {
        delete (*it);
    }
    m_animations.clear();
}

void Entity::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("name");
    if(attb) {
        m_name = attb->Value();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    while (child) {
        if(strcmp(child->Name(), "animation") == 0) {
            Animation* animation = new Animation(this);
            animation->loadXML(child);
            addAnimation(animation);
        }
        child = child->NextSiblingElement();
    }
}

void Entity::addAnimation(Animation* a_animation) {
    m_animations.push_back(a_animation);
}

std::ostream& operator<< (std::ostream& out, const Entity& entity){
    if(entity.m_animations.size() >= 1) {
        out << "local anim = {" << endl;
        for(vector<Animation*>::const_iterator it = entity.m_animations.begin(); it != entity.m_animations.end(); it++) {
            out << *(*it);
        }
        out << "}" << endl << endl;
        out << "return anim";
    }
    return out;
}


