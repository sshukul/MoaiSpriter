//
//  map.cpp
//  Spriter2Moai
//

#include "map.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include "characterMap.h"
#include "file.h"

using namespace std;
using namespace tinyxml2;

void Map::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("folder");
    if(attb) {
        m_folder = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("file");
    if(attb) {
        m_file = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("target_folder");
    if(attb) {
        m_target_folder = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("target_file");
    if(attb) {
        m_target_file = attb->IntValue();
    }
}

std::ostream& operator<< (std::ostream& out, Map& map) {
    out << "\t\t{" << endl;
    
    out << "\t\t\t['file'] = '" << map.m_owner->getFileName(map.getFolder(), map.getFile()) << "'";
    if(map.getTargetFolder() >= 0 && map.getTargetFile() >= 0) {
        out << "," << endl;
        out << "\t\t\t['target_file'] = '" << map.m_owner->getFileName(map.getTargetFolder(), map.getTargetFile()) << "'";
    }
    
    out << endl << "\t\t}";
    return out;
}