/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "HkxRigTool.h"
#include "Animation/include/Rig.h"
#include "Util/include/Environment.h"
#include "Stream/include/Stream.h"
#include "Config.h"

using namespace CoS;
//---------------------------------------------------------------------------
static char s_buf[8192];
static const char* ToolName = "HkxRigTool";
static Environment s_env;
//---------------------------------------------------------------------------
HkxRigTool::HkxRigTool(const HkxRigToolParams& params)
: m_params(params)
{
}
//---------------------------------------------------------------------------
HkxRigTool::~HkxRigTool()
{
}
//---------------------------------------------------------------------------
bool HkxRigTool::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRigTool::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRigTool::process(
	Rig& rig, Stream& src)
{
	// right now, we just pack whatever is in the src stream into the 
	// resource -- typically should always be DDS texture file data
	rig.load(src);
	return true;
}
