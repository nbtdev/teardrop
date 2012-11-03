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

#if !defined(COSGFXSHADERLIB_INCLUDED)
#define COSGFXSHADERLIB_INCLUDED

#include <map>

namespace CoS
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

#endif // COSGFXSHADERLIB_INCLUDED
