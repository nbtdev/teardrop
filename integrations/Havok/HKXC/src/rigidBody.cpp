/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxRigidBodyTool.h"
#include "Physics/BodyHavok.h"
#include "Serialization/ResourceSerializer.h"
#include "Stream/FileStream.h"
#include "hkxc.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool doRigidBody(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxRigidBodyToolParams bodyParams;
	bodyParams.bVerbose = params.bVerbose;

	HkxRigidBodyTool bodyTool(bodyParams);
	bodyTool.initialize();

	FileStream fs;
	if (!fs.open(inputFilename, READ|BINARY))
	{
		// TODO: log it?
		return false;
	}

	BodyHavok body;
	if (bodyTool.process(body, fs))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		body.serialize(ser);
	}
	body.destroy();
	bodyTool.destroy();

	return true;
}
