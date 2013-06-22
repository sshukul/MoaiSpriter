//
//  sourceUtil.c
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/17/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//
#include "sourceUtil.h"
#include "glUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ShaderSource* srcLoadSource(const char* filepathname)
{
	ShaderSource* source = (ShaderSource*) calloc(sizeof(ShaderSource), 1);
	
	// Check the file name suffix to determine what type of shader this is
	const char* suffixBegin = filepathname + strlen(filepathname) - 4;
	
	if(0 == strncmp(suffixBegin, ".fsh", 4))
	{
		source->shaderType = GL_FRAGMENT_SHADER;
	}
	else if(0 == strncmp(suffixBegin, ".vsh", 4))
	{
		source->shaderType = GL_VERTEX_SHADER;
	}
	else
	{
		// Unknown suffix
		source->shaderType = 0;
	}
	
	FILE* curFile = fopen(filepathname, "r");
	
	// Get the size of the source
	fseek(curFile, 0, SEEK_END);
	GLsizei fileSize = (GLsizei)ftell (curFile);
	
	// Add 1 to the file size to include the null terminator for the string
	source->byteSize = fileSize + 1;
	
	// Alloc memory for the string
	source->string = (GLchar*)malloc(source->byteSize);
	
	// Read entire file into the string from beginning of the file
	fseek(curFile, 0, SEEK_SET);
	fread(source->string, 1, fileSize, curFile);
	
	fclose(curFile);
	
	// Insert null terminator
	source->string[fileSize] = 0;
	
	return source;
}

void srcDestroySource(ShaderSource* source)
{
	free(source->string);
	free(source);
}
