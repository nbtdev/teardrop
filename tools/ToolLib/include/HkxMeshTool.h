/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDHKXMESHTOOL_INCLUDED)
#define TDHKXMESHTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
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

#endif // TDHKXMESHTOOL_INCLUDED
