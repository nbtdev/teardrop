/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxRigTool.h"
#include "Animation//RigHavok.h"
#include "Serialization/ResourceSerializer.h"
#include "Stream/FileStream.h"
#include "hkxc.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool doRig(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxRigToolParams rigParams;
	rigParams.bVerbose = params.bVerbose;

	HkxRigTool rigTool(rigParams);
	rigTool.initialize();

	FileStream fs;
	if (!fs.open(inputFilename, READ|BINARY))
	{
		// TODO: log it?
		return false;
	}

	RigHavok rig;
	if (rigTool.process(rig, fs))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		rig.serialize(ser);
	}
	rig.destroy();
	rigTool.destroy();

	return true;
}
