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

#include "Config.h"
#include "ToolLib/include/HkxSceneTool.h"
#include "hkxc.h"
#include "direct.h"

using namespace CoS;

//---------------------------------------------------------------------------
bool doScene(
	hkRootLevelContainer* container, 
	const RCParams& params, 
	const StringSet& options,
	const char* outFile
	)
{
	HkxSceneToolParams sceneParams;
	sceneParams.bVerbose = params.bVerbose;
	sceneParams.bMergeMeshesByMaterial = true;
	sceneParams.outFile = outFile;

	std::string stem(outFile);
	stem = stem.substr(0, stem.find_last_of("\\/")+1);

	std::string outDir;
	for (StringSet::const_iterator it = options.begin(); it != options.end(); ++it)
	{
		if (strstr(it->c_str(), "--outputDir"))
		{
			outDir = it->substr(it->find_first_of('=')+1);
			break;
		}
	}

	if (!outDir.length())
	{
		outDir = ".\\";
	}

	// first make sure they exist
	_mkdir(stem.c_str());
	_mkdir((stem+outDir).c_str());

	sceneParams.outDir = outDir.c_str();

	HkxSceneTool sceneTool(sceneParams);
	sceneTool.initialize(container);

	if (sceneTool.process())
	{
	}

	return true;
}
