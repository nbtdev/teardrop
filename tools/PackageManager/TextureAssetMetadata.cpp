/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TextureAssetMetadata.h"
#include "Asset/TextureAsset.h"
#include "FreeImage.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(TextureAssetMetadata);

TextureAssetMetadata::TextureAssetMetadata()
{
}

TextureAssetMetadata::TextureAssetMetadata(TextureAsset* asset)
{
	mObject = asset;
	PropertyChanged.bind(fastdelegate::MakeDelegate(this, &TextureAssetMetadata::onPropertyChanged));
}

TextureAssetMetadata::~TextureAssetMetadata()
{
	PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &TextureAssetMetadata::onPropertyChanged));
}

void TextureAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}

void TextureAssetMetadata::generateThumbnail()
{
	FIBITMAP* fibm = FreeImage_Load(FreeImage_GetFileType(getSourcePath()), getSourcePath());

	if (fibm) {
		BITMAPINFO* bmi = FreeImage_GetInfo(fibm);

		if (bmi->bmiHeader.biBitCount != 32) {
			fibm = FreeImage_ConvertTo32Bits(fibm);
			bmi = FreeImage_GetInfo(fibm);
		}

		// make thumbnail
		FIBITMAP* thumb = FreeImage_MakeThumbnail(fibm, 128);
		if (thumb) {
			BITMAPINFO* thumbInfo = FreeImage_GetInfo(thumb);
			int w = thumbInfo->bmiHeader.biWidth;
			int h = thumbInfo->bmiHeader.biHeight;

			if (thumbInfo->bmiHeader.biSizeImage) {
				mThumb.resize(w, h, thumbInfo->bmiHeader.biSizeImage);

				FIMEMORY* mem = FreeImage_OpenMemory((BYTE*)mThumb.data(), mThumb.length());
				FreeImage_SaveToMemory(FIF_PNG, thumb, mem);
				FreeImage_CloseMemory(mem);
			}
			else {
				FIMEMORY* mem = FreeImage_OpenMemory();
				FreeImage_SaveToMemory(FIF_PNG, thumb, mem);
				BYTE* data;
				DWORD sz;
				FreeImage_AcquireMemory(mem, &data, &sz);
				mThumb.resize(w, h, sz);
				mThumb.setData(data, sz);
				FreeImage_CloseMemory(mem);
			}
		}
	}
}