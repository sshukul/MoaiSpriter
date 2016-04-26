//
//  object.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__object__
#define __Spriter2Moai__object__

#include "tinyxml2.h"

class Object {
private:
    int m_id;
    int m_folder;
    int m_file;
    float m_x;
    float m_y;
    float m_angle;
    float m_scaleX;
    float m_scaleY;
    unsigned int m_time;
    int m_spin;
    float m_pivot_x = 0.0;
    float m_pivot_y = 0.0;
    
public:
    Object(): m_id(-1), m_folder(-1), m_file(-1), m_x(0), m_y(0), m_angle(0), m_scaleX(1), m_scaleY(1), m_time(0), m_spin(1), m_pivot_x(0.0), m_pivot_y(0.0) {}
    Object(const Object& rhs);
    ~Object() {}
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline int getFolder() const {return m_folder;}
    inline void setFolder(int a_folder) {m_folder = a_folder;}
    inline int getFile() const {return m_file;}
    inline void setFile(int a_file) {m_file = a_file;}
    inline float getX() const {return m_x;}
    inline void setX(float a_x) {m_x = a_x;}
    inline float getY() const {return m_y;}
    inline void setY(float a_y) {m_y = a_y;}
    inline float getAngle() const {return m_angle;}
    inline void setAngle(float a_angle) {m_angle = a_angle;}
    inline float getScaleX() const {return m_scaleX;}
    inline void setScaleX(float a_scale) {m_scaleX = a_scale;}
    inline float getScaleY() const {return m_scaleY;}
    inline void setScaleY(float a_scale) {m_scaleY = a_scale;}
    inline unsigned int getTime() const {return m_time;}
    inline void setTime(unsigned int a_time) {m_time = a_time;}
    inline int getSpin() const {return m_spin;}
    inline void setSpin(int a_spin) {m_spin = a_spin;}
    inline float getPivotX() const {return m_pivot_x;}
    inline void setPivotX(float a_pivot_x) {m_pivot_x = a_pivot_x;}
    inline float getPivotY() const {return m_pivot_y;}
    inline void setPivotY(float a_pivot_y) {m_pivot_y = a_pivot_y;}
    
    bool equals(const Object& o);
    Object* clone();
    
    void loadXML(const tinyxml2::XMLElement* a_element);
};

#endif /* defined(__Spriter2Moai__object__) */
