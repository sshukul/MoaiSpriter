//
//  entity.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef Spriter2Moai_entity_h
#define Spriter2Moai_entity_h

#include <vector>
#include <string>
#include "tinyxml2.h"

using namespace std;

class Animation;
class CharacterMap;
class SpriterData;

class Entity {
private:
    SpriterData* m_owner;
    int m_id;
    string m_name;
    vector<Animation*> m_animations;
    vector<CharacterMap*> m_character_maps;

public:
    
    Entity(): m_owner(NULL), m_id(-1), m_name("") {}
    Entity(SpriterData* a_owner): m_owner(a_owner), m_id(-1), m_name("") {}
    ~Entity();
    
    
    inline SpriterData* getOwner() const {return m_owner;}
    inline void setOwner(SpriterData* a_owner) {m_owner = a_owner;}
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    inline bool hasCharacterMaps () {return m_character_maps.size() >= 1;}

    void loadXML(const tinyxml2::XMLElement* a_element);
    void writeCharacterMaps (std::ostream& out);
    
    friend std::ostream& operator<< (std::ostream& out, const Entity& entity);
    
    void addAnimation(Animation* a_animation);
    void addCharacterMap(CharacterMap* a_character_map);
};

std::ostream& operator<< (std::ostream& out, const Entity& entity);

#endif
