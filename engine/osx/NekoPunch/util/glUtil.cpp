//
//  glUtil.c
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/17/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

#include "glUtil.h"
#include <cstdio>
#include <stdlib.h>

bool loadShaders(unsigned int& program, ShaderSource* vertexSource, ShaderSource* fragmentSource,
                ShaderAttrib* attribArray, unsigned int attribArrayLength) {
    //GLint logLength, status;
    GLuint vertShader, fragShader;
    
    // String to pass to glShaderSource
    GLchar* sourceString = NULL;
    
    // Determine if GLSL version 140 is supported by this context.
    //  We'll use this info to generate a GLSL shader source string
    //  with the proper version preprocessor string prepended
    float  glLanguageVersion;
    
#ifdef IOS
    sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "OpenGL ES GLSL ES %f", &glLanguageVersion);
#elif OSX
    sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &glLanguageVersion);
#endif
    
    // GL_SHADING_LANGUAGE_VERSION returns the version standard version form
    //  with decimals, but the GLSL version preprocessor directive simply
    //  uses integers (thus 1.10 should 110 and 1.40 should be 140, etc.)
    //  We multiply the floating point number by 100 to get a proper
    //  number for the GLSL preprocessor directive
    GLuint version = 100 * glLanguageVersion;
    
    // Get the size of the version preprocessor string info so we know
    //  how much memory to allocate for our sourceString
    const GLsizei versionStringSize = sizeof("#version 123\n");
    
    // Create shader program
    program = glCreateProgram();
    
    //
    // Create and compile the vertex shader
    
    // Allocate memory for the source string including the version preprocessor information
    sourceString = (GLchar*)malloc(vertexSource->byteSize + versionStringSize);
    // Prepend our vertex shader source string with the supported GLSL version so
    //  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
    sprintf(sourceString, "#version %d\n%s", version, vertexSource->string);
    compileShader(vertShader, GL_VERTEX_SHADER, sourceString);
    free(sourceString);
    sourceString = NULL;
    
    //
    // Create and compile fragment shader
    
    // Allocate memory for the source string including the version preprocessor information
    sourceString = (GLchar*)malloc(fragmentSource->byteSize + versionStringSize);
    // Prepend our vertex shader source string with the supported GLSL version so
    //  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
    sprintf(sourceString, "#version %d\n%s", version, fragmentSource->string);
    compileShader(fragShader, GL_FRAGMENT_SHADER, sourceString);    
    free(sourceString);
    sourceString = NULL;
    
    // Attach vertex shader to program.
    glAttachShader(program, vertShader);    
    // Attach fragment shader to program.
    glAttachShader(program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    for(int i=0; i<attribArrayLength; i++) {
        glBindAttribLocation(program, attribArray[i].index, attribArray[i].name);
    }
    
    // Link program.
    if (!linkProgram(program)) {
#if defined(DEBUG)
        printf("Failed to link program: %d\n", program);
#endif
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
        return false;
    }
        
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program, fragShader);
        glDeleteShader(fragShader);
    }
    return true;
}

bool compileShader(GLuint& shader, GLenum type,  const GLchar* source) {
    GLint status;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s\n", log);
        free(log);
    }
#endif
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(shader);
        return false;
    }
    return true;
}

bool linkProgram(GLuint prog) {
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log:\n%s\n", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    return true;
}

bool validateProgram(GLuint prog) {
    GLint status;
    glValidateProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program validate log:\n%s\n", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return false;
    }
    return true;
}
