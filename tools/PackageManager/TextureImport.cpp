/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "AssetImport.h"
#include "Metadata.h"
#include "Asset/TextureAsset.h"
#include "FreeImage.h"
#include "squish.h"
#include <cstring>

namespace Teardrop {
namespace Tools {

TextureAsset* importTexture(const char* filepath, TextureAssetType type)
{
	TextureAsset* asset = 0;

	FIBITMAP* fibm = FreeImage_Load(FreeImage_GetFileType(filepath), filepath);
	if (fibm) {
		BITMAPINFO* bmi = FreeImage_GetInfo(fibm);
		int w = bmi->bmiHeader.biWidth;
		int h = bmi->bmiHeader.biHeight;
		int depth = bmi->bmiHeader.biBitCount;

		asset = new TextureAsset;
		asset->setWidth(w);
		asset->setHeight(h);

		if (type == TEXTUREASSET_TYPE_UNCOMPRESSED) {
			int bpp = 0;
			if (depth == 8)
				bpp = 1;
			if (depth == 16)
				bpp = 2;

			int sz = w * h * bpp;

			if (sz) {
				void* data = asset->createData(sz);
				memcpy(data, FreeImage_GetBits(fibm), sz);
			}
			else {
				delete asset;
				asset = 0;
			}
		}
		else {
			if (depth != 32) {
				fibm = FreeImage_ConvertTo32Bits(fibm);
				bmi = FreeImage_GetInfo(fibm);
			}

			int sz = squish::GetStorageRequirements(w, h, squish::kDxt3);
			void* data = asset->createData(sz);
			squish::CompressImage((squish::u8*)FreeImage_GetBits(fibm), w, h, data, squish::kDxt3);
		}

		asset->setFormat(TextureAsset::TEXFMT_BC2);

		FreeImage_Unload(fibm);
	}

	return asset;
}

} // namespace Tools
} // namespace Teardrop
