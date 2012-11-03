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
#include "Link.h"
#include "Serialization/ResourceLibrary.h"
#include "Stream/FileStream.h"
#include "Stream/TextReader.h"
#include "Memory/Memory.h"
#include "Memory/MemoryRegion.h"
#include "ToolLib/include/Logger.h"
#include <string>
#include <sstream>

using namespace CoS;
//---------------------------------------------------------------------------
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
COS_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
extern const char* pUsage;
static const char* TOOLNAME = "Link";
//---------------------------------------------------------------------------
void parseArgs(LinkParams& params, int argc, char* argv[])
{
	for (int i=1; i<argc; ++i)
	{
		if (argv[i]) // hyperparanoia...
		{
			if (*argv[i] == '@')
			{
				params.cmdFileName = argv[i]+1;
			}
			else
			{
				if (*argv[i] == '-' && i < argc-1)
				{
					switch(argv[i][1])
					{
					case 'o': // output filename
						params.libName = argv[++i];
						break;
					}
				}
				else
				{
					params.options.insert(std::string(argv[i]));
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	LinkParams params;

	// parse args
	parseArgs(params, argc, argv);
	if (!params.cmdFileName || !params.libName)
	{
		Logger::logMessage(Logger::ERR, TOOLNAME, pUsage);
		return 1;
	}

	params.bVerbose = (params.options.find("-v") != params.options.end());

	// open the output file(s) -- one for the lib, and one for its import lib
	FileStream libStrm, expStrm;
	std::string expName(params.libName);
	expName = expName.substr(0, expName.find_last_of('.'));
	expName += ".imp";

	if (!libStrm.open(params.libName, WRITE|BINARY|TRUNCATE))
	{
		std::stringstream ss;
		ss << "Could not open output file '" << 
			params.libName << "', exiting" << std::endl;

		Logger::logMessage(Logger::ERR, TOOLNAME, ss.str().c_str());
		return 1;
	}

	if (!expStrm.open(expName.c_str(), WRITE|BINARY|TRUNCATE))
	{
		std::stringstream ss;
		ss << "Could not open exports file '" << 
			expName << "', exiting" << std::endl;

		Logger::logMessage(Logger::ERR, TOOLNAME, ss.str().c_str());
		return 1;
	}

	// open the input file containing the files to link
	FileStream cmdFile;
	if (!cmdFile.open(params.cmdFileName, READ))
	{
		std::stringstream ss;
		ss << "Could not open command file '" << 
			params.cmdFileName << "', exiting" << std::endl;

		Logger::logMessage(Logger::ERR, TOOLNAME, ss.str().c_str());
		return 1;
	}

	// iterate through the list of files and add each one to the library
	ResourceLibrary lib;
	lib.initialize();

	TextReader r(cmdFile);
	char buf[1024];

	while (r.getline(buf, 1024))
	{
		if (!buf[0] || buf[0]=='#')
		{
			// ignore empty/comment lines
			continue;
		}

		FileStream res;
		if (!res.open(buf, READ|BINARY))
		{
			std::stringstream ss;
			ss << "Could not find input file '" << 
				buf << "', exiting" << std::endl;

			Logger::logMessage(Logger::ERR, TOOLNAME, ss.str().c_str());
			return 1;
		}

		lib.addResource(res);
		res.close();
	}

	cmdFile.close();

	lib.serialize(libStrm, expStrm);

	libStrm.close();
	expStrm.close();

	lib.destroy();

	return 0;
}

