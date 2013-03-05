/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxSceneTool.h"
#include "hkxc.h"
#include "direct.h"

using namespace Teardrop;

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
