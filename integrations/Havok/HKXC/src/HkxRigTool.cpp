/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "HkxRigTool.h"
#include "Animation/Rig.h"
#include "Util/Environment.h"
#include "Stream/Stream.h"
#include "Config.h"

using namespace Teardrop;
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
