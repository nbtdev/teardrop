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

#if !defined(COSGFXRENDERSTATE_INCLUDED)
#define COSGFXRENDERSTATE_INCLUDED

#include "GfxMaterial.h"
#include "GfxShaderAutoConst.h"
#include "Math/Matrix44.h"
#include "Math/Vector4.h"
#include "Util/_String.h"

namespace CoS
{
	class GfxMesh;
	struct Environment;
	class GfxMeshInstance;
	class GfxTextureStage;
	class Vector4;
	class GfxRenderTargetD3D9;

	class GfxRenderState
	{
	public:
		Environment& env;

		struct SamplerState
		{
			void* pTexture;
			bool enabled;
			unsigned int minFilter;
			unsigned int magFilter;
			unsigned int mipFilter;
			unsigned int addrU;
			unsigned int addrV;
			unsigned int addrW;
		};

#pragma pack(push)
#pragma pack(16)
		struct AutoConstant
		{
			AutoConst semantic;
			const char* string;
			size_t len; // number of float4 slots this constant takes up
			size_t isArray; // whether this is an array constant
			Vector4* pData; // pointer to beginning of this constant's data

			// matrix-specific flags
			size_t needsUpdate;
			size_t invert;
			size_t transpose;
			size_t deps[3];
		};
#pragma pack(pop)

		// this is the mapping from string semantic to semantic enum
		typedef std::map<String, AutoConst> AutoConstByName;
		AutoConstByName m_acLut;
		// this is a constant descriptor table indexed by the semantic enum
		static AutoConstant m_autoConst[];//[AC_MAX];
		// convenience string-to-semantic translation method, returns AC_NONE if not found
		AutoConst getAutoConstBySemantic(const String& semantic);
		// convenience const-char*-to-semantic translation method, returns AC_NONE if not found or const char* is null
		AutoConst getAutoConstBySemantic(const char* semantic);
		// get a pointer to the start of the data for this constant
		const Vector4* getConstant(AutoConst semantic);
		// this is where the actual constant data lives; pData in the constant descriptor points into here
		Vector4 m_constData[1024]; // arbitrary limit for now

		static const int MAX_SAMPLERS = 8;
		SamplerState samplers[MAX_SAMPLERS];

		// matrix cache
		float world[16];
		float view[16];
		float proj[16];
		float viewproj[16];

		// vertex constant cache
		float vfConst[256][4];

		// pixel constant cache
		float pfConst[32][4];

		// material state
		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];

		// buffer write state
		bool colorWrite;

		// lighting state
		bool lighting;
		bool depthWrite;
		bool alphaBlend;
		bool depthCheck;
		unsigned int srcBlend;
		unsigned int destBlend;
		unsigned int depthFunc;
		unsigned int depthBias;
		GfxMaterial::CullMode cullMode;
		GfxMaterial::FillMode fillMode;

		// shader state
		IDirect3DVertexShader9* pVS;
		IDirect3DPixelShader9* pPS;

		// render target state
		GfxRenderTargetD3D9* pRT[4]; // max 4 MRT

		// set the primary matrices
		void setWorldMatrix(const Matrix44& m);
		void setViewMatrix(const Matrix44& m);
		void setProjectionMatrix(const Matrix44& m);
		void setViewProjectionMatrix(const Matrix44& m);
		void setLightViewProjMatrix(const Matrix44& m, size_t index);

		//
		// helper methods
		// 
		void setWVP(const Matrix44& wvp);
		void setWorldView(const Matrix44& wv);
		void setViewInverse(const Matrix44& vi);
		void setTextureMatrix(const Matrix44& mat, size_t idx);
		void setMatrixPaletteEntry(const Matrix44& ent, size_t index);
		void setLightPos(const Vector4& pos, size_t index);
		void setLightDir(const Vector4& dir, size_t index);
		void setLightColor(const Vector4& col, size_t index);
		void setLightSpecularColor(const Vector4& col, size_t index);
		void setWorldAmbient(const Vector4& col);
		void setCameraDepth(const Vector4& depth);
		void setMaterialDiffuse(const Vector4& col);
		void setMaterialAmbient(const Vector4& col);
		void setMaterialSpecular(const Vector4& col);
		void setMaterialEmissive(const Vector4& col);
		void setColorWrite(bool bWrite);
		bool getColorWrite();

		// send the values that are constant across all renders this frame
		void updateVSInvariantConstants();
		void updatePSInvariantConstants();

		// send the values that can change per-render
		void updateVSConstants();
		void updateBoneConstants();
		void updatePSConstants();

		// API render state interface
		void setDepthWrite(bool write);
		void setDepthCheck(bool check);
		void setDepthBias(unsigned int bias);
		void setDepthFunc(unsigned int func);
		void enableAlphaBlend(bool enable);
		void setSrcBlend(unsigned int blend);
		void setDestBlend(unsigned int blend);
		void setCullMode(GfxMaterial::CullMode mode);
		void setFillMode(GfxMaterial::FillMode mode);

		// sampler control
		void setSampler(unsigned int s, GfxTextureStage* pStage);
		// sets a sampler directly
		bool setSampler(unsigned int s, GfxTexture* pTex);

		// RT control
		void setRenderTarget(size_t idx, GfxRenderTargetD3D9* pRT, bool force=false);

		//
		// c'tor
		//
		GfxRenderState(Environment& e);

		DECLARE_GFX_ALLOCATOR();

	private:
		GfxRenderState(const GfxRenderState& other);
		GfxRenderState& operator=(const GfxRenderState& other);
	};
}

#endif // COSGFXRENDERSTATE_INCLUDED
