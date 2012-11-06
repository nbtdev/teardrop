/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXSHADERD3D9_INCLUDED)
#define GFXSHADERD3D9_INCLUDED

#include "Gfx/GfxShaderAutoConst.h"
#include "Gfx/GfxShader.h"
#include "Util/_String.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	class Vector4;
	class GfxRenderState;
	class GfxShaderConstantTable;
	class String;
	class Matrix44;

	class GfxShaderD3D9 :
		public GfxShader
	{
		ID3DXEffectPool* m_pFXPool;
		ID3DXEffect* m_pFX;
		D3DXHANDLE m_technique;
		D3DXHANDLE m_techniqueShadowCast;
		D3DXHANDLE m_techniqueShadowRecv;
		String m_source; // in case we need to recompile the shader
		String m_fullpath; // in case someone wants to save the shader?
		String m_errs; // in case we need to recompile the shader

	public:
		GfxShaderD3D9(ID3DXEffectPool* pFXPool);
		~GfxShaderD3D9();

		enum ShaderVariant
		{
			SV_DEFAULT,
			SV_SHADOWCASTER,
			SV_SHADOWRECEIVER,
		};

		bool initialize(
			const String& source, 
			String& errs);
		void destroy();

		// will set this effect's parameters first (numMatrices is a temp hack until 
		// we figure out how to make multi-part animated models use the same 
		// bind pose matrices)
		void setState(
			GfxRenderState* pState, 
			GfxShaderConstantTable* pSource, 
			size_t numMatrices=0);
		bool setTechnique(ShaderVariant sv); // sets technique to the one requested during initialization, returns false if failed
		ID3DXEffect* getFX();
		void releaseD3DResources();

		void setSource(const String& source) { m_source = source; }
		void setFullPath(const String& fullpath) { m_fullpath = fullpath; }

		// GfxShader implementation
		const String& getSource() const { return m_source; }
		const String& getCompileErrors() const { return m_errs; }
		const String& getFullPath() const { return m_fullpath; }
		bool compile(const String& source, String& errs);

		DECLARE_GFX_ALLOCATOR();

	private:

		struct ConstantTableEntry
		{
			unsigned __int64 name;	// hash of text name for all constants
			AutoConst semantic;		// AC_*** const semantic
			size_t numRows;			// number of float constant rows used in the shader (?)
			size_t numCols;			// number of elements used in the float4 constant
			size_t maxRows;			// max rows in an element decl (?)
			void* handle;			// handle to render-API-specific shader constant
		};
		typedef std::vector<ConstantTableEntry> Constants;
		Constants m_constants;

		static bool UDless(const ConstantTableEntry& r1, const ConstantTableEntry& r2);
		bool initD3D9(String& errs);
		void setupParams(Constants& constants);
	};
}

#endif // GFXSHADERD3D9_INCLUDED
