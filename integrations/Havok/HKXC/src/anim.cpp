/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxAnimTool.h"
#include "Animation/AnimationHavok.h"
#include "Serialization/ResourceSerializer.h"
#include "Stream/FileStream.h"
#include "hkxc.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool doAnim(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxAnimToolParams animParams;
	animParams.bVerbose = params.bVerbose;

	HkxAnimTool animTool(animParams);
	animTool.initialize();

	FileStream fs;
	if (!fs.open(inputFilename, READ|BINARY))
	{
		// TODO: log it?
		return false;
	}

	AnimationHavok anim;
	if (animTool.process(anim, fs))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		anim.serialize(ser);
	}
	anim.destroy();
	animTool.destroy();

	return true;
}
