/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageManager.h"
#include "Metadata.h"
#include "Asset/TextureAsset.h"
#include "FreeImage.h"
#include "squish.h"

namespace Teardrop {
namespace Tools {

TextureAsset* importTexture(const char* filepath, TextureAssetType type)
{
	TextureAsset* asset = 0;

	FIBITMAP* fibm = FreeImage_Load(FreeImage_GetFileType(filepath), filepath);
	if (fibm) {
		BITMAPINFO* bmi = FreeImage_GetInfo(fibm);

		if (bmi->bmiHeader.biBitCount != 32) {
			fibm = FreeImage_ConvertTo32Bits(fibm);
			bmi = FreeImage_GetInfo(fibm);
		}

		asset = new TextureAsset;
		int w = bmi->bmiHeader.biWidth;
		int h = bmi->bmiHeader.biHeight;
		int sz = squish::GetStorageRequirements(w, h, squish::kDxt3);
		void* data = asset->createData(sz);
		squish::CompressImage((squish::u8*)fibm->data, w, h, data, squish::kDxt3);
	}

	return asset;
}

} // namespace Tools
} // namespace Teardrop
