//
//  glUtil.h
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/17/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

#ifndef NyanTest4_glUtil_h
#define NyanTest4_glUtil_h

#ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
#elif OSX
    #import <OpenGL/gl.h>
    #import <OpenGL/glext.h>
#endif
#include "sourceUtil.h"



#ifdef IOS

#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES

#elif OSX

#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE

#endif


// Attribute index.
enum ATTRIB_INDEX
{
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

typedef struct _ShaderAttrib {
    ATTRIB_INDEX index;
    char* name;
} ShaderAttrib;

bool loadShaders(unsigned int& program, ShaderSource* vertexSource, ShaderSource* fragmentSource,
                 ShaderAttrib* attribArray, unsigned int attribArrayLength);
bool compileShader(GLuint& shader, GLenum type,  const GLchar* source);
bool linkProgram(GLuint prog);
bool validateProgram(GLuint prog);
// GLint buildGLProgram(shaderSource* vertexSource, shaderSource* fragmentSource, bool hasNormal, bool hasTexcoord);


static inline const char * GetGLErrorString(GLenum error)
{
	const char *str;
	switch( error )
	{
		case GL_NO_ERROR:
			str = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			str = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			str = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			str = "GL_INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			str = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			str = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			str = "(ERROR: Unknown Error Enum)";
			break;
	}
	return str;
}

#endif
