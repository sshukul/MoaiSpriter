//
//  folder.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef Spriter2Moai_folder_h
#define Spriter2Moai_folder_h

#include "tinyxml2.h"
#include <string>
#include <vector>

using namespace std;

class File;

class Folder {
private:
    int m_id;
    string m_name;
    vector<File*> m_files;
    
public:
    Folder(): m_id(-1), m_name("") {}
    Folder(const Folder& a_folder) {
        m_id = a_folder.m_id;
        m_name = a_folder.m_name;
    }
    
    ~Folder();
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    
    const Folder& operator= (const Folder& a_folder) {
        m_id = a_folder.m_id;
        m_name = a_folder.m_name;
        return *this;
    }
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    void addFile(File* a_file);
    
    string getFilePath(unsigned int a_fileIndex);
    File* getFile(unsigned int a_fileIndex);
};

#endif
