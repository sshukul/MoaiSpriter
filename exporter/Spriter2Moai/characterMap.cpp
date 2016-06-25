//
//  characterMap.cpp
//  Spriter2Moai
//

#include "characterMap.h"
#include "entity.h"
#include "spriterData.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include "map.h"

using namespace std;
using namespace tinyxml2;
using namespace boost::filesystem;

CharacterMap::~CharacterMap() {
    for (vector<Map*>::iterator it = m_maps.begin(); it != m_maps.end(); it++) {
        delete (*it);
    }
    m_maps.clear();
}

void CharacterMap::addMap(Map* a_map) {
    m_maps.push_back(a_map);
}

void CharacterMap::loadXML(const tinyxml2::XMLElement* a_element) {
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
        if(strcmp(child->Name(), "map") == 0) {
            Map* map = new Map(this);
            map->loadXML(child);
            addMap(map);
        }
        child = child->NextSiblingElement();
    }
}

string CharacterMap::getFileName(unsigned int a_folderIndex, unsigned int a_fileIndex) {
    SpriterData* spriter = m_owner->getOwner();
    
    string szPath = spriter->getFilePath(a_folderIndex, a_fileIndex);
    if(szPath.length() <= 0)
        return szPath;
    
    path file(szPath);
    return file.filename().c_str();
}

std::ostream& operator<< (std::ostream& out, CharacterMap& characterMap) {
    cout << "Converting character-map " << characterMap.m_name;
    out << "\t['" << characterMap.m_name << "'] = {" << endl;
    
    if(characterMap.m_maps.size() > 0) {
        for(vector<Map*>::const_iterator it = characterMap.m_maps.begin(); it != characterMap.m_maps.end(); ++it) {
            out << *(*it);
            if(it+1 != characterMap.m_maps.end()) {
                out << "," << endl;
            }
        }
    }
    
    out << endl << "\t}";
    cout << " ... Done.\n";
    return out;
}