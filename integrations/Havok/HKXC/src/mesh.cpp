/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "HkxMeshTool.h"
#include "Gfx/GfxMesh.h"
#include "Serialization/ResourceSerializer.h"
#include "hkxc.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool doMesh(
	hkRootLevelContainer* container, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxMeshToolParams meshParams;
	meshParams.bVerbose = params.bVerbose;
	meshParams.bMergeMeshesByMaterial = (options.find("--merge") != options.end());

	HkxMeshTool meshTool(meshParams);
	meshTool.initialize(container);

	GfxMesh mesh;
	mesh.initialize();
	if (meshTool.process(mesh))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		mesh.serialize(ser);
	}
	mesh.destroy();
	meshTool.destroy();

	return true;
}
