/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(__GLHEADERS_H__)
#define __GLHEADERS_H__

#if defined(_WIN32) || defined(_WIN64)
	#include <GL/gl.h>
	#include <GL/wglext.h>
	#include <GL/glext.h>
	#define GL_GETPROCADDRESS wglGetProcAddress
	#define GL_UBYTE_PTR
	#define GL_SIZEIPTR uintptr_t
#elif defined(__linux__)
	#define GL_GLEXT_PROTOTYPES 1
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glext.h>
	#define GL_GETPROCADDRESS glXGetProcAddress
	#define GL_UBYTE_PTR (GLubyte*)
	#define GL_SIZEIPTR GLsizeiptr
#endif

#endif // __GLHEADERS_H__
