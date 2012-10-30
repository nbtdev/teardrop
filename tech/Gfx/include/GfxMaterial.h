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

#if !defined(COSGFXMATERIAL_INCLUDED)
#define COSGFXMATERIAL_INCLUDED

namespace CoS
{
	struct GfxMaterialImpl;
	class GfxMaterialBase;
	class GfxTextureStage;
	class GfxTexture;
	struct Environment;
	class ResourceSerializer;

	class GfxMaterial
	{
		DECLARE_SERIALIZABLE(GfxMaterial);

	public:
		//! normal c'tor (cannot fail)
		GfxMaterial();
		//! placement c'tor (cannot fail)
		GfxMaterial(int);
		//! d'tor (cannot fail)
		~GfxMaterial();

		//! create and initialize the material (anything that can fail)
		bool initialize();
		//! destroy the material
		bool destroy();

		enum CullMode
		{
			CULL_NONE = 0,
			CULL_CCW, // default
			CULL_CW,

			CULL_FORCE_BYTE=0xFF,
		};

		enum FillMode
		{
			FILLMODE_NONE = 0,
			FILLMODE_POINT,
			FILLMODE_WIREFRAME,
			FILLMODE_SOLID,

			FILLMODE_FORCE_BYTE=0xFF,
		};

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
		unsigned int getDiffuse() const;
		unsigned int getAmbient() const;
		unsigned int getSpecular() const;
		unsigned int getEmissive() const;

		bool getColorWrite() const;
		bool getDepthCheck() const;
		bool getDepthWrite() const;
		unsigned char getDepthBias() const;
		bool getVertexColors() const;
		bool isLit() const;
		bool isAlphaBlended() const;
		size_t getNumLights() const;
		size_t getMaxNumTextureStages() const;
		size_t getNumTextureStages() const;
		GfxTextureStage* addTextureStage();
		GfxTextureStage* getTextureStage(size_t index);
		const GfxTextureStage* getTextureStage(size_t index) const;
		bool isTextureStageEnabled(size_t index) const;
		CullMode getCullMode() const;
		FillMode getFillMode() const;
		CustomShader getCustomShader() const;
		unsigned __int64 getHashCode();
		bool isTransparent() const;
		void checkForAllTextures();

		/** mutators
		*/
		void setDiffuse(unsigned int c);
		void setAmbient(unsigned int c);
		void setSpecular(unsigned int c);
		void setEmissive(unsigned int c);

		void setColorWrite(bool bEnabled);
		void setDepthCheck(bool bEnabled);
		void setDepthWrite(bool bEnabled);
		void setDepthBias(unsigned char bias);
		void setAlphaBlended(bool blend);
		void setVertexColors(bool bEnabled);
		void setNumLights(size_t numLights);
		bool setTextureStage(size_t index, bool bEnabled);
		void setCullMode(CullMode culling);
		void setFillMode(FillMode fillMode);
		void setCustomShader(CustomShader shader);

		GfxMaterial& operator=(const GfxMaterial& other);

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);
		
		// called by texture stage if changed
		void notify();

		DECLARE_GFX_ALLOCATOR();

	private:
		unsigned int m_diffuse;
		unsigned int m_specular;
		unsigned int m_ambient;
		unsigned int m_emissive;
		__int64 m_hashCode; // used for shader lookups
		bool m_bColorWrite;
		bool m_bDepthWrite;
		bool m_bStencilWrite;
		bool m_bVertexColors;
		bool m_bDepthCheck;
		bool m_bStencilCheck;
		bool m_bDepthFunc;
		bool m_bHasAllTextures;
		bool m_bAlphaBlend;
		unsigned char m_numLights;
		unsigned short m_special; // to flag different custom shaders
		CullMode m_cullMode;
		FillMode m_fillMode;
		unsigned char m_depthBias;
		char m_reserved[1];

		
		SerialPointerArray<GfxTextureStage> m_pTextureStages;

		void recalcHashCode(); 

		//! NOT IMPLEMENTED
		GfxMaterial(const GfxMaterial& other);
	};
}

#endif // COSGFXMATERIAL_INCLUDED
