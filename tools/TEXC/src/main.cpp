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
#include "Stream/include/FileStream.h"
#include "Gfx/include/GfxRenderer.h"
#include "Gfx/include/GfxTexture.h"
#include "Gfx/include/GfxBitmap.h"
#include "Util/include/Environment.h"
#include "Util/include/Hash.h"
#include "Util/include/_String.h"
#include "Serialization/include/ResourceSerializer.h"
#include "Memory/include/Memory.h"
#include "Memory/include/MemoryRegion.h"

#include "texc.h"

using namespace CoS;
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
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
COS_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
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
	CoS::setGfxAllocator(GetCRTAllocator());
	CoS::String::setAllocator(GetCRTAllocator());

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