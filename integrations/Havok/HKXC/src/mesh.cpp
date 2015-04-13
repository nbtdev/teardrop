/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
