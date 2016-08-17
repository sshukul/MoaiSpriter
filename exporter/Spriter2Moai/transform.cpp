//
//  transform.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/22/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include "transform.h"
#include <math.h>


// This is for rotating untranslated points and offsetting them to a new origin.
static void rotate_point(float& x, float& y, float angle, float origin_x, float origin_y, bool flipped)
{
    if(flipped)
        angle = -angle;
    
    float s = sinf(angle*(float)(M_PI/180.0f));
    float c = cosf(angle*(float)(M_PI/180.0f));
    float xnew = (x * c) - (y * s);
    float ynew = (x * s) + (y * c);
    xnew += origin_x;
    ynew += origin_y;
    
    x = xnew;
    y = ynew;
}


Transform::Transform()
: x(0.0f), y(0.0f), angle(0.0f), scale_x(1.0f), scale_y(1.0f), alpha(1.0f)
{}

Transform::Transform(float x, float y, float angle, float scale_x, float scale_y, int spin, float alpha,
                     string curve_type, float c1, float c2, float c3, float c4)
: x(x), y(y), angle(angle), scale_x(scale_x), scale_y(scale_y), spin(spin), alpha(alpha),
curve_type(curve_type), c1(c1), c2(c2), c3(c3), c4(c4)
{}

bool Transform::operator==(const Transform& t) const
{
    return (x == t.x && y == t.y && angle == t.angle && scale_x == t.scale_x && scale_y == t.scale_y);
}

bool Transform::operator!=(const Transform& t) const
{
    return !(*this == t);
}

void Transform::lerp(const Transform& transform, float t, int spin)
{
    if(transform.curve_type == "quadratic") {
        t = quadratic(0, c1, 1, t);
    } else if(transform.curve_type == "cubic") {
        t = cubic(0, c1, c2, 1, t);
    } else if(transform.curve_type == "quartic") {
        t = quartic(0, c1, c2, c3, 1, t);
    } else if(transform.curve_type == "quintic") {
        t = quintic(0, c1, c2, c3, c4, 1, t);
    }
    
    x = lerp(x, transform.x, t);
    y = lerp(y, transform.y, t);
    alpha = lerp(alpha, transform.alpha, t);
    
    // 'spin' is based on what you are coming from (key1)
    if(spin != 0)
    {
        if(spin > 0 && angle > transform.angle)
            angle = lerp(angle, transform.angle + 360, t);
        else if(spin < 0 && angle < transform.angle)
            angle = lerp(angle, transform.angle - 360, t);
        else
            angle = lerp(angle, transform.angle, t);
    }
    if(angle > 360) {
        angle = angle - 360;
    }
    
    scale_x = lerp(scale_x, transform.scale_x, t);
    scale_y = lerp(scale_y, transform.scale_y, t);
}

void Transform::apply_parent_transform(const Transform& parent)
{
    x *= parent.scale_x;
    y *= parent.scale_y;
    
    bool flipped = ((parent.scale_x < 0) != (parent.scale_y < 0));
    rotate_point(x, y, parent.angle, parent.x, parent.y, flipped);
    
    angle += parent.angle;
    scale_x *= parent.scale_x;
    scale_y *= parent.scale_y;
    
    rotationAngle += parent.rotationAngle;
    if(rotationAngle < 0) {
        spin = -1;
    } else if(rotationAngle == 0) {
        spin = 0;
    } else {
        spin = 1;
    }
    
    if(curve_type == "") {
        curve_type = parent.curve_type;
        c1 = parent.c1;
        c2 = parent.c2;
        c3 = parent.c3;
        c4 = parent.c4;
    }
}