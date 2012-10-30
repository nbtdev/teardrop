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

#if !defined(COSHKXMESHTOOL_INCLUDED)
#define COSHKXMESHTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace CoS
{
	class GfxMesh;
	class GfxRenderer;

	struct HkxMeshToolParams
	{
		HkxMeshToolParams()
		{
			bVerbose = false;
			bMergeMeshesByMaterial = false;
			resId = 0;
		}

		bool bVerbose;
		bool bMergeMeshesByMaterial;
		unsigned __int64 resId;
	};

	class HkxMeshTool
	{
		hkRootLevelContainer* m_pHkx;
		const HkxMeshToolParams& m_params;
		GfxRenderer* m_pGfx;

		// processing statistics
		size_t m_numVerts;
		size_t m_numTris;
		size_t m_numMaterials;
		size_t m_numSubMeshes;

	public:
		//! normal c'tor (cannot fail)
		HkxMeshTool(const HkxMeshToolParams& params);
		//! d'tor (cannot fail)
		~HkxMeshTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize(hkRootLevelContainer* container);
		//! destroy
		bool destroy();
		//! strip out the mesh from the hkx and pack it into internal file format
		bool process(GfxMesh& destMesh, hkRootLevelContainer& destContainer);
		//! process the mesh from the container given in initialize()
		bool process(GfxMesh& destMesh);
		
		/** accessors
		*/

		/** mutators
		*/

	private:
		const char* getStats();
		void _process(GfxMesh& destMesh);

		//! NOT IMPLEMENTED
		HkxMeshTool(const HkxMeshTool& other);
		HkxMeshTool& operator=(const HkxMeshTool& other);
	};
}

#endif // COSHKXMESHTOOL_INCLUDED
