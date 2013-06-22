//
//  folder.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <stdio.h>
#include "folder.h"
#include "file.h"

using namespace tinyxml2;


Folder::~Folder() {
    for (vector<File*>::iterator it = m_files.begin(); it != m_files.end(); it++) {
        delete (*it);
    }
    m_files.clear();
}

void Folder::loadXML(const tinyxml2::XMLElement* a_element) {
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
        if(strcmp(child->Name(), "file") == 0) {
            File* file = new File();
            file->loadXML(child);
            addFile(file);
        }
        child = child->NextSiblingElement();
    }
}

void Folder::addFile(File* a_file) {
    m_files.push_back(a_file);
}

string Folder::getFilePath(unsigned int a_fileIndex) {
    if (a_fileIndex >= m_files.size())
        return "";
    return m_files[a_fileIndex]->getName();
}

File* Folder::getFile(unsigned int a_fileIndex){
    if (a_fileIndex >= m_files.size())
        return NULL;
    return m_files[a_fileIndex];
}





