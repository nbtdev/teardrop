/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/
#include "HkxAnimTool.h"
#include "Animation/Animation.h"
#include "Util/Environment.h"
#include "Stream/Stream.h"
#include "Config.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_buf[8192];
static const char* ToolName = "HkxAnimTool";
static Environment s_env;
//---------------------------------------------------------------------------
HkxAnimTool::HkxAnimTool(const HkxAnimToolParams& params)
: m_params(params)
{
}
//---------------------------------------------------------------------------
HkxAnimTool::~HkxAnimTool()
{
}
//---------------------------------------------------------------------------
bool HkxAnimTool::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxAnimTool::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxAnimTool::process(
	Animation& anim, Stream& src)
{
	// right now, we just pack whatever is in the src stream into the 
	// resource -- typically should always be DDS texture file data
	anim.load(src);
	return true;
}
