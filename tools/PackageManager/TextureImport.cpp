/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
