/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXSHADERLIB_INCLUDED)
#define TDGFXSHADERLIB_INCLUDED

#include <map>

namespace Teardrop
{
	struct Environment;
	class GfxSubMesh;
	class GfxMaterial;
	class GfxShaderD3D9;
	class String;

	class GfxShaderLib
	{
		Environment& m_env;

		typedef std::map<unsigned __int64, GfxShaderD3D9*> ShaderLUT;
		ShaderLUT m_lut;

	public:
		GfxShaderLib(Environment& env);
		~GfxShaderLib();

		bool initialize();
		bool destroy();

		// find a shader by hash
		GfxShaderD3D9* findShader(unsigned __int64 hashCode) const;
		// find a shader by material and submesh
		GfxShaderD3D9* findShader(
			/*in*/GfxSubMesh* pSubMesh, 
			/*in*/GfxMaterial* pMaterial, 
			/*in*/const char* pFilename,
			/*out*/unsigned __int64& hash) const;
		// create or find a shader based on mesh and material properties (generated shader), or with an optional shader override (name)
		GfxShaderD3D9* createShader(GfxSubMesh* pSubMesh, GfxMaterial* pMaterial, const char* pFilename=0);

		DECLARE_GFX_ALLOCATOR();

	private:
		void dumpShaderSource(unsigned __int64 hash, const char* src);

		ID3DXEffectPool* m_pEffectPool;

		//! NOT IMPLEMENTED
		GfxShaderLib(const GfxShaderLib& other);
		GfxShaderLib& operator=(const GfxShaderLib& other);
	};
}

#endif // TDGFXSHADERLIB_INCLUDED
