//
//  point.hpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 25/04/16.
//  Copyright © 2016 Samuel Cartwright. All rights reserved.
//

#include <stdio.h>

class Point
{
public:
    float x, y; // coordinates of the point
    Point (float ax, float ay) {
        x = ax;
        y = ay;
    }
};
