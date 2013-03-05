/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxRagdollTool.h"
#include "Ragdoll/RagdollHavok.h"
#include "Serialization/ResourceSerializer.h"
#include "Stream/FileStream.h"
#include "hkxc.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool doRagdoll(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxRagdollToolParams ragdollParams;
	ragdollParams.bVerbose = params.bVerbose;

	HkxRagdollTool ragdollTool(ragdollParams);
	ragdollTool.initialize();

	FileStream fs;
	if (!fs.open(inputFilename, READ|BINARY))
	{
		// TODO: log it?
		return false;
	}

	RagdollHavok ragdoll;
	if (ragdollTool.process(ragdoll, fs))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		ragdoll.serialize(ser);
	}
	ragdoll.destroy();
	ragdollTool.destroy();

	return true;
}
