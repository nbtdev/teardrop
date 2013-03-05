/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HKXMESHTOOL_INCLUDED)
#define HKXMESHTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
{
	class GfxMesh;
	class GfxRenderer;

	struct HkxSceneToolParams
	{
		HkxSceneToolParams()
		{
			bVerbose = false;
			bMergeMeshesByMaterial = true;
			outDir = 0;
			outFile = 0;
		}

		bool bVerbose;
		bool bMergeMeshesByMaterial;
		const char* outDir;
		const char* outFile;
	};

	class HkxSceneTool
	{
		hkRootLevelContainer* m_pHkx;
		const HkxSceneToolParams& m_params;
		GfxRenderer* m_pGfx;

		// processing statistics
		size_t m_numVerts;
		size_t m_numTris;
		size_t m_numMaterials;
		size_t m_numSubMeshes;

	public:
		//! normal c'tor (cannot fail)
		HkxSceneTool(const HkxSceneToolParams& params);
		//! d'tor (cannot fail)
		~HkxSceneTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize(hkRootLevelContainer* container);
		//! destroy
		bool destroy();
		//! strip out the mesh from the hkx and pack it into internal file format
		bool process();
		
		/** accessors
		*/

		/** mutators
		*/

	private:
		const char* getStats();
		void processScene(const hkxScene& pScene);

		//! NOT IMPLEMENTED
		HkxSceneTool(const HkxSceneTool& other);
		HkxSceneTool& operator=(const HkxSceneTool& other);
	};
}

#endif // HKXMESHTOOL_INCLUDED
