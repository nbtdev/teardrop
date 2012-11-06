/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "HkxRigidBodyTool.h"
#include "Physics/Body.h"
#include "Util/Environment.h"
#include "Stream/Stream.h"
#include "Config.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_buf[8192];
static const char* ToolName = "HkxRigTool";
static Environment s_env;
//---------------------------------------------------------------------------
HkxRigidBodyTool::HkxRigidBodyTool(const HkxRigidBodyToolParams& params)
: m_params(params)
{
}
//---------------------------------------------------------------------------
HkxRigidBodyTool::~HkxRigidBodyTool()
{
}
//---------------------------------------------------------------------------
bool HkxRigidBodyTool::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRigidBodyTool::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRigidBodyTool::process(
	Body& body, Stream& src)
{
	// right now, we just pack whatever is in the src stream into the 
	// resource -- typically should always be DDS texture file data
	body.load(src);
	return true;
}
