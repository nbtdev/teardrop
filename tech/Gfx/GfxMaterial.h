/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXMATERIAL_INCLUDED)
#define TDGFXMATERIAL_INCLUDED

#include "Reflection/Reflection.h"
#include "Math/Vector4.h"
#include "Gfx/GfxTextureStage.h"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Teardrop
{
	struct GfxMaterialImpl;
	class GfxMaterialBase;
	class GfxTextureStage;
	class GfxTexture;
	struct Environment;
	class ResourceSerializer;
	class TextureAsset;

	class GfxMaterial : public Reflection::Object
	{
	public:
		TD_CLASS(GfxMaterial, Object);
		TD_CLASS_CREATABLE();
		TD_COMPLEX_PROPERTY(Diffuse, "Material Diffuse Color", Vector4, "(0.5,0.5,0.5,0.5)", ColorEditor);
		TD_COMPLEX_PROPERTY(Ambient, "Material Ambient Color", Vector4, "(0.5,0.5,0.5,0.5)", ColorEditor);
		TD_COMPLEX_PROPERTY(Specular, "Material Specular Color", Vector4, "(0.5,0.5,0.5,0.5)", ColorEditor);
		TD_COMPLEX_PROPERTY(Emissive, "Material Emissive Color", Vector4, "(0.5,0.5,0.5,0.5)", ColorEditor);
		TD_NESTED_PROPERTY(DiffuseMap, "Diffuse map texture stage", GfxTextureStage);
		TD_PROPERTY(ColorWrite, "Enable/disable writing to the color buffer", bool, true, 0);
		TD_PROPERTY(DepthCheck, "Enable/disable checking against the depth buffer (alpha blending)", bool, true, 0);
		TD_PROPERTY(DepthWrite, "Enable/disable writing to the depth buffer", bool, true, 0);
		TD_PROPERTY(DepthBias, "Set depth bias for this material", int, 0, 0);
		TD_PROPERTY(VertexColors, "Enable/disable blending of mesh vertex colors when rendering with this material", bool, false, 0);
		TD_PROPERTY(Lit, "Enable/disable dynamic lighting when rendering with this material", bool, true, 0);
		TD_PROPERTY(AlphaBlending, "Enable/disable alpha blending with target buffer when rendering with this material", bool, false, 0);

		enum CullMode {
			CULL_NONE,
			CULL_CCW,
			CULL_CW,
		};

		enum FillMode {
			FILLMODE_NONE,
			FILLMODE_POINT,
			FILLMODE_WIREFRAME,
			FILLMODE_SOLID,
		};

		TD_ENUM_PROPERTY(CullMode, "Set culling mode for this material", CullMode, CULL_CCW);
		TD_ENUM_PROPERTY(FillMode, "Set fill mode for this material", FillMode, FILLMODE_SOLID);

		//! normal c'tor (cannot fail)
		GfxMaterial();
		//! placement c'tor (cannot fail)
		GfxMaterial(int);
		//! d'tor (cannot fail)
		~GfxMaterial();

        GfxMaterial(const GfxMaterial& other) = delete;

		//! create and initialize the material (anything that can fail)
		bool initialize();
		//! destroy the material
		bool destroy();

		enum CustomShader
		{
			SHADER_NO_CUSTOM = 0,
			SHADER_TERRAIN,
			SHADER_MACRO,
			SHADER_PROP,
			SHADER_VFX,

			SHADER_FORCE_SHORT = 0xFFFF,
		};
		
		/** accessors
		*/
		size_t getNumLights() const;
		size_t getMaxNumTextureStages() const;
		size_t getNumTextureStages() const;
		GfxTextureStage* addTextureStage();
		GfxTextureStage* getTextureStage(size_t index);
		const GfxTextureStage* getTextureStage(size_t index) const;
		bool isTextureStageEnabled(size_t index) const;
		CustomShader getCustomShader() const;
        uint64_t getHashCode();
		bool isTransparent() const;
		void checkForAllTextures();

		/** mutators
		*/
		void setNumLights(size_t numLights);
		bool setTextureStage(size_t index, bool bEnabled);
		void setCustomShader(CustomShader shader);

		GfxMaterial& operator=(const GfxMaterial& other);
		
		// called by texture stage if changed
		void notify();

		DECLARE_GFX_ALLOCATOR();

	private:
        int64_t m_hashCode; // used for shader lookups
		bool m_bStencilWrite;
		bool m_bStencilCheck;
		bool m_bHasAllTextures;
		unsigned char m_numLights;
		unsigned short m_special; // to flag different custom shaders
        std::vector<GfxTextureStage*> mTextureStages;

		void recalcHashCode(); 
	};
}

#endif // TDGFXMATERIAL_INCLUDED
