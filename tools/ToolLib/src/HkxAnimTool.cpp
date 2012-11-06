/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
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
