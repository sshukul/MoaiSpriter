//
//  file.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef Spriter2Moai_file_h
#define Spriter2Moai_file_h

#include <string>
#include "tinyxml2.h"

using namespace std;

class File {
private:
    int m_id;
    string m_name;
    unsigned int m_width;
    unsigned int m_height;
    float m_pivot_x = 0.0;
    float m_pivot_y = 0.0;
    
public:
    File(): m_id(-1), m_name(""), m_width(0), m_height(0)  {}
    File(const File& a_folder) {
        m_id = a_folder.m_id;
        m_name = a_folder.m_name;
        m_width = a_folder.m_width;
        m_height = a_folder.m_height;
        m_pivot_x = a_folder.m_pivot_x;
        m_pivot_y = a_folder.m_pivot_y;
    }
    
    ~File() {}
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    inline unsigned int getWidth() const {return m_width;}
    inline void setWidth(unsigned int a_width) {m_width = a_width;}
    inline unsigned int getHeight() const {return m_height;}
    inline void setHeight(unsigned int a_height) {m_height = a_height;}
    inline float getPivotX() const {return m_pivot_x;}
    inline void setPivotX(float a_pivot_x) {m_pivot_x = a_pivot_x;}
    inline float getPivotY() const {return m_pivot_y;}
    inline void setPivotY(float a_pivot_y) {m_pivot_y = a_pivot_y;}
    
    const File& operator= (const File& a_folder) {
        m_id = a_folder.m_id;
        m_name = a_folder.m_name;
        m_width = a_folder.m_width;
        m_height = a_folder.m_height;
        m_pivot_x = a_folder.m_pivot_x;
        m_pivot_y = a_folder.m_pivot_y;
        return *this;
    }
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
};

#endif
