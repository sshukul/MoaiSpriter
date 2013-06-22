//
//  types.h
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/30/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

#ifndef NyanTest4_types_h
#define NyanTest4_types_h

typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

// analogous to the b2Color struct used by box2D (added for portability)
struct color3f
{
	color3f() {}
	color3f(float32 r, float32 g, float32 b) : r(r), g(g), b(b) {}
	void Set(float32 ri, float32 gi, float32 bi) { r = ri; g = gi; b = bi; }
	float r, g, b;
};

#endif
