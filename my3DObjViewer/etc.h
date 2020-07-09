#pragma once

#ifndef _MYENGINE_COMMON_ETC_

#define _MYENGINE_COMMON_ETC_



#define BUFFER_OFFSET(pointer) ((void*)(pointer))

unsigned int app_time();

struct ShaderInfo
{
	int shaderType;
	const char* shaderFile;
};

struct ShaderInfo2
{
	GLuint* pShaderId;
	int shaderType;
	const char* shaderFile;
};

#endif
