/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <set>
#include <map>
#include <string>
#include <algorithm>

#include "Config.h"
#include "ToolLib/include/TextureTool.h"
#include "Stream/FileStream.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxTexture.h"
#include "Gfx/GfxBitmap.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Util/_String.h"
#include "Serialization/ResourceSerializer.h"
#include "Memory/Memory.h"
#include "Memory/MemoryRegion.h"

#include "texc.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static const char* pUsage=
"usage: texc <input.xxx> <output.tex> [flags]\n"
"\n"
"  Type of the input texture is determined by the value of 'xxx'. Current \n"
"  valid input extensions:\n"
"\n"
"    tga         TGA format texture\n"
"\n"
"  (Optional) flags appear after filename arguments. Current valid flags:\n"
"\n"
"    FLAGS\n"
"    ----------------------------------------------------------------------\n"
"    --verbose      Verbose output\n"
"    --dxt1         Use DXT1 compression on the output texture\n"
"    --dxt3         Use DXT3 compression on the output texture\n"
"    --dxt5         Use DXT5 compression on the output texture\n"
"    --normal       The texture is a normal map\n"
"    --nomips       Generate no mip maps in the texture\n"
"\n"
;

//---------------------------------------------------------------------------
TD_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
TD_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
//---------------------------------------------------------------------------
void usage()
{
	fprintf(stderr, pUsage);
}
//---------------------------------------------------------------------------
typedef std::set<std::string> StringSet;
static StringSet s_flagsLUT;
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	Environment::get().isOffline = 1;
	Teardrop::setGfxAllocator(GetCRTAllocator());
	Teardrop::String::setAllocator(GetCRTAllocator());

	// must have at least an input and output filename
	if (argc < 2)
	{
		usage();
		return 1;
	}

	std::string files[2];

	// scan input for flags
	int idx = 0;
	for (int i=1; i<argc; ++i)
	{
		if (*argv[i] == '-')
		{
			s_flagsLUT.insert(std::string(argv[i]));
		}
		else
		{
			files[idx++] = argv[i];
		}
	}

	// have to have at least an input file
	if (!files[0].length())
	{
		usage();
		return 1;
	}

	// default output filename: change ext to .tex and place it in current working dir
	std::string fname(files[0]);
	fname = fname.substr(fname.find_last_of("\\/")+1);

	if (!files[1].length())
	{
		fname = fname.substr(0, fname.find_last_of('.'));
		fname += ".tex";
		files[1] = fname;
	}

	TextureToolParams params;
	params.bVerbose = (s_flagsLUT.find("--verbose") != s_flagsLUT.end());
	params.bGenerateMips = !(s_flagsLUT.find("--nomips") != s_flagsLUT.end());
	params.bNormalMap = !(s_flagsLUT.find("--normal") != s_flagsLUT.end());

	if (s_flagsLUT.find("--dxt1") != s_flagsLUT.end())
	{
		params.compType = TextureToolParams::DXT1;
	}
	else if (s_flagsLUT.find("--dxt3") != s_flagsLUT.end())
	{
		params.compType = TextureToolParams::DXT3;
	}
	if (s_flagsLUT.find("--dxt5") != s_flagsLUT.end())
	{
		params.compType = TextureToolParams::DXT5;
	}

	FileStream strm, src;
	// open an output filestream
	if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
	{
		fprintf(stderr, "Could not open output file '%s', exiting...\n", files[1].c_str());
		return 2;
	}

	// open the input filestream
	if (!src.open(files[0].c_str(), READ|BINARY))
	{
		fprintf(stderr, "Could not open input file '%s', exiting...\n", files[0].c_str());
		return 2;
	}

	Environment::get().isOffline = 1;
	GfxRenderer* pRend = GfxRenderer::allocate(Environment::get());
	Environment::get().pRenderer = pRend;

	TextureTool tool(params);
	GfxTexture* pTex = pRend->createTexture();
	
	bool bRtn = tool.process(*pTex, src);

	// generate id for this resource from the filename
	fname = files[1].substr(files[1].find_last_of("\\/")+1);
	unsigned __int64 id = hashString64(fname.c_str());

	ResourceSerializer ser(strm);
	ser.setId(id);
	ser.start();
	pTex->serialize(ser);
	ser.finalize();

	strm.close();

	pTex->destroy();
	delete pTex;

	GfxRenderer::deallocate(pRend);
	return 0;
}