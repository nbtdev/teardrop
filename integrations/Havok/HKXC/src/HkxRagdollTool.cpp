/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "HkxRagdollTool.h"
#include "Ragdoll/Ragdoll.h"
#include "Util/Environment.h"
#include "Stream/Stream.h"
#include "Config.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_buf[8192];
static const char* ToolName = "HkxRagdollTool";
static Environment s_env;
//---------------------------------------------------------------------------
HkxRagdollTool::HkxRagdollTool(const HkxRagdollToolParams& params)
: m_params(params)
{
}
//---------------------------------------------------------------------------
HkxRagdollTool::~HkxRagdollTool()
{
}
//---------------------------------------------------------------------------
bool HkxRagdollTool::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRagdollTool::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool HkxRagdollTool::process(
	Ragdoll& ragdoll, Stream& src)
{
	// right now, we just pack whatever is in the src stream into the 
	// resource -- typically should always be DDS texture file data
	ragdoll.load(src);
	return true;
}
