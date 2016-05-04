//
//  objectRef.h
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#ifndef __Spriter2Moai__objectRef__
#define __Spriter2Moai__objectRef__

#include "tinyxml2.h"

class ObjectRef {
private:
    int m_id;
    int m_parent;
    int m_timeline;
    int m_key;
    int m_zIndex;
    
public:
    ObjectRef(): m_id(-1), m_parent(-1), m_timeline(-1), m_key(-1), m_zIndex(0) {}
    ~ObjectRef() {}
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline int getParent() const {return m_parent;}
    inline void setParent(int a_parent) {m_parent = a_parent;}
    inline int getTimeline() const {return m_timeline;}
    inline void setTimeline(int a_timeline) {m_timeline = a_timeline;}
    inline int getKey() const {return m_key;}
    inline void setKey(int a_key) {m_key = a_key;}
    inline int getZIndex() const {return m_zIndex;}
    inline void setZIndex(int a_zIndex) {m_zIndex = a_zIndex;}
    
    void loadXML(const tinyxml2::XMLElement* a_element);
};

#endif /* defined(__Spriter2Moai__objectRef__) */
