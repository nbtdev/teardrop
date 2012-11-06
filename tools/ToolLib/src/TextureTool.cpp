/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "TextureTool.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxTexture.h"
#include "Util/Environment.h"
#include "Stream/Stream.h"
#include "Config.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_buf[8192];
static const char* ToolName = "TextureTool";
static Environment s_env;
//---------------------------------------------------------------------------
TextureTool::TextureTool(const TextureToolParams& params)
: m_params(params)
{
}
//---------------------------------------------------------------------------
TextureTool::~TextureTool()
{
}
//---------------------------------------------------------------------------
bool TextureTool::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool TextureTool::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool TextureTool::process(
	GfxTexture& texture, Stream& src)
{
	// right now, we just pack whatever is in the src stream into the 
	// resource -- typically should always be DDS texture file data
	texture.loadTextureData(src);
	return true;
}
