/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(__GLHEADERS_H__)
#define __GLHEADERS_H__

#if defined(_WIN32) || defined(_WIN64)
	#include <GL/glew.h>
	#include <GL/wglew.h>
	#include <GL/gl.h>
    #include <GL/wglext.h>
    #include <GL/glext.h>
#elif defined(__linux__)
    #include <GL/glew.h>
    #include <GL/glxew.h>
    #include <GL/gl.h>
	#include <GL/glx.h>
	#include <GL/glext.h>
#endif

#endif // __GLHEADERS_H__
