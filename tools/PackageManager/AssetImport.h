/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#if !defined(ASSETIMPORT_INCLUDED)
#define ASSETIMPORT_INCLUDED

namespace Teardrop 
{
	class TextureAsset;
	class LandscapeAsset;
	class StaticMeshAsset;

	namespace Tools
	{
		class ImportedAsset;

		enum TextureAssetType {
			TEXTUREASSET_TYPE_BCX,			// compress to BCx (default in editor)
			TEXTUREASSET_TYPE_NATIVE,		// compress to native type for target 
			TEXTUREASSET_TYPE_UNCOMPRESSED,	// no compression (heightfields, etc)

			TEXTUREASSET_TYPE_MAX,
		};

		enum LandscapeAssetType {
			LANDSCAPEASSET_TYPE_L3DT,		// Landscape created in L3DT

			LANDSCAPEASSET_TYPE_MAX,
		};

		TextureAsset* importTexture(const char* filepath, TextureAssetType type);
		LandscapeAsset* importLandscape(/*out*/ImportedAsset& imp, /*in*/const char* filepath, LandscapeAssetType type);
		StaticMeshAsset* importStaticMesh(/*out*/ImportedAsset& imp, /*in*/const char* filepath);
	}
}

#endif // ASSETIMPORT_INCLUDED
