//
//  SpriterData.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__SpriterData__
#define __Spriter2Moai__SpriterData__

#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;

class XMLElement;
class Folder;
class Entity;
class TagList;
class Tag;
class File;

class SpriterData {
private:
    float m_version;
    string m_generator;
    string m_generatorVersion;
    vector<Folder*> m_folders;
    vector<Entity*> m_entities;
    vector<TagList*> m_tagLists;
    
public:
    SpriterData () {}
    ~SpriterData();
    
    void loadXML(const tinyxml2::XMLElement* element);
    
    friend std::ostream& operator<< (std::ostream& out, const SpriterData& spriter);
    
    void addFolder(Folder* a_folder);
    void addEntity(Entity* a_entity);
    void addTagList(TagList* a_tagList);
    void populateTagNameFromTagList(Tag* a_tag);
    
    string getFilePath(unsigned int a_folderIndex, unsigned int a_fileIndex);
    File* getFile(unsigned int a_folderIndex, unsigned int a_fileIndex);
};

std::ostream& operator<< (std::ostream& out, const SpriterData& spriter);

#endif /* defined(__Spriter2Moai__SpriterData__) */
