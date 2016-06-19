//
//  object.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/18/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include "object.h"

using namespace tinyxml2;

void Object::loadXML(const tinyxml2::XMLElement* a_element) {
    const XMLAttribute* attb = a_element->FindAttribute("id");
    if(attb) {
        m_id = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("time");
    if(attb) {
        m_time = attb->IntValue();
    }
    
    attb = a_element->FindAttribute("spin");
    if(attb) {
        m_spin = attb->IntValue();
    }
    
    const tinyxml2::XMLElement* child = a_element->FirstChildElement();
    if(strcmp(child->Name(), "object") == 0) {
        attb = child->FindAttribute("folder");
        if(attb) {
            m_folder = attb->IntValue();
        }
        
        attb = child->FindAttribute("file");
        if(attb) {
            m_file = attb->IntValue();
        }
        
        attb = child->FindAttribute("x");
        if(attb) {
            m_x = attb->FloatValue();
        }
        
        attb = child->FindAttribute("y");
        if(attb) {
            m_y = attb->FloatValue();     // invert y
        }
        
        attb = child->FindAttribute("angle");
        if(attb) {
            m_angle = attb->FloatValue();      // invert angle
        }
        
        attb = child->FindAttribute("scale_x");
        if(attb) {
            m_scaleX = attb->FloatValue();
        }
        
        attb = child->FindAttribute("scale_y");
        if(attb) {
            m_scaleY = attb->FloatValue();
        }
        
        attb = child->FindAttribute("pivot_x");
        if(attb) {
            m_pivot_x = attb->FloatValue();
        }
        
        attb = child->FindAttribute("pivot_y");
        if(attb) {
            m_pivot_y = attb->FloatValue();
        }
        
        attb = child->FindAttribute("a");
        if(attb) {
            m_alpha = attb->FloatValue();
        }
    }
}

bool Object::equals(const Object& o) {
    if (o.getX() != this->getX()) {
        return false;
    } else if (o.getY() != this->getY()) {
        return false;
    } else if (o.getAngle() != this->getAngle()) {
        return false;
    } else if (o.getScaleX() != this->getScaleX()) {
        return false;
    } else if (o.getScaleY() != this->getScaleY()) {
        return false;
    } else if (o.getFile() != this->getFile()) {
        return false;
    } else if (o.getFolder() != this->getFolder()) {
        return false;
    } else if (o.getAlpha() != this->getAlpha()) {
        return false;
    } else if (o.getSpin() != this->getSpin()) {
        return false;
    } else {
        return true;
    }
}

Object* Object::clone() {
    Object* object = new Object();
    object->setX(this->getX());
    object->setY(this->getY());
    object->setTime(this->getTime());
    object->setScaleX(this->getScaleX());
    object->setScaleY(this->getScaleY());
    object->setId(this->getId());
    object->setFile(this->getFile());
    object->setFolder(this->getFolder());
    object->setSpin(this->getSpin());
    object->setAngle(this->getAngle());
    object->setPivotX(this->getPivotX());
    object->setPivotY(this->getPivotY());
    object->setAlpha(this->getAlpha());
    return object;
}