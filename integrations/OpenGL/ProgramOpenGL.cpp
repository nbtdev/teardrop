/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ProgramOpenGL.h"
#include "FragmentShaderOpenGL.h"
#include "VertexShaderOpenGL.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

Program::Program() {

}

Program::~Program() {

}

bool Program::initialize(VertexShader* aVS, FragmentShader* aFS) 
{
	mVS = aVS;
	mFS = aFS;

	mProgramName = glCreateProgram();

	return true;
}

bool Program::destroy() 
{
	if (mProgramName)
		glDeleteProgram(mProgramName);

	return true;
}

void Program::apply()
{
	assert(mProgramName);
	glUseProgram(mProgramName);
}

void Program::disable()
{
	glUseProgram(0);
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop