//
//  sourceUtil.h
//  Functions for loading source files for shaders.
//
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/17/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

#ifndef NyanTest4_sourceUtil_h
#define NyanTest4_sourceUtil_h

#ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
#elif OSX
    #import <OpenGL/gl.h>
    #import <OpenGL/glext.h>
#endif

typedef struct _shaderSource
{
	GLchar* string;
	GLsizei byteSize;	
	GLenum shaderType; // Vertex or Fragment
	
} ShaderSource;

ShaderSource* srcLoadSource(const char* filepathname);

void srcDestroySource(ShaderSource* source);


#endif
