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
#include "characterMap.h"
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
        if(strcmp(child->Name(), "character_map") == 0) {
            CharacterMap* characterMap = new CharacterMap(this);
            characterMap->loadXML(child);
            addCharacterMap(characterMap);
        }
        child = child->NextSiblingElement();
    }
}

void Entity::addAnimation(Animation* a_animation) {
    m_animations.push_back(a_animation);
}

void Entity::addCharacterMap(CharacterMap* a_character_map) {
    m_character_maps.push_back(a_character_map);
}

void Entity::writeCharacterMaps(std::ostream& out) {
    if(m_character_maps.size() >= 1) {
        for(vector<CharacterMap*>::const_iterator it = m_character_maps.begin(); it != m_character_maps.end(); it++) {
            out << *(*it);
            if(it+1 != m_character_maps.end()) {
                out << "," << endl;
            }
        }
    }
}

std::ostream& operator<< (std::ostream& out, const Entity& entity){
    size_t totalAnimations = entity.m_animations.size();
    int animationCounter = 0;
    if(entity.m_animations.size() >= 1) {
        for(vector<Animation*>::const_iterator it = entity.m_animations.begin(); it != entity.m_animations.end(); it++) {
            animationCounter++;
            
            out << *(*it);
        
            if(animationCounter < totalAnimations)
                out << "," << endl;
        }
    }
    return out;
}



