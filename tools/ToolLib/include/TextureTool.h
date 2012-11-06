/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTURETOOL_INCLUDED)
#define TEXTURETOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
{
	class GfxTexture;
	class GfxRenderer;
	class Stream;

	struct TextureToolParams
	{
		enum CompressionType
		{
			NONE=0,
			DXT1,
			DXT3,
			DXT5,
		};

		TextureToolParams()
		{
			memset(this, 0, sizeof(TextureToolParams));
		}

		bool bVerbose;
		bool bNormalMap;
		bool bGenerateMips;
		CompressionType compType;
	};

	class TextureTool
	{
		const TextureToolParams& m_params;
		GfxRenderer* m_pGfx;

		// processing statistics

	public:
		//! normal c'tor (cannot fail)
		TextureTool(const TextureToolParams& params);
		//! d'tor (cannot fail)
		~TextureTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize();
		//! destroy
		bool destroy();
		//! process using the existing settings
		bool process(GfxTexture& texture, Stream& src);
		
		/** accessors
		*/

		/** mutators
		*/

	private:

		//! NOT IMPLEMENTED
		TextureTool(const TextureTool& other);
		TextureTool& operator=(const TextureTool& other);
	};
}

#endif // TEXTURETOOL_INCLUDED
