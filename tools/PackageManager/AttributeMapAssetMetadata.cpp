/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AttributeMapAssetMetadata.h"
#include "Asset/AttributeMapAsset.h"
#include "FreeImage.h"
#include <cstring>

using namespace std::placeholders;
using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(AttributeMapAssetMetadata);

AttributeMapAssetMetadata::AttributeMapAssetMetadata()
{
}

AttributeMapAssetMetadata::AttributeMapAssetMetadata(AttributeMapAsset* asset)
{
	mObject = asset;
    PropertyChanged.bind(std::bind(&AttributeMapAssetMetadata::onPropertyChanged, this, _1));
}

AttributeMapAssetMetadata::~AttributeMapAssetMetadata()
{
    PropertyChanged.unbind(std::bind(&AttributeMapAssetMetadata::onPropertyChanged, this, _1));
}

void AttributeMapAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}

void AttributeMapAssetMetadata::generateThumbnail()
{
	if (!mObject) 
		return;

	AttributeMapAsset* amAsset = static_cast<AttributeMapAsset*>(mObject);

	// what we have is a two-channel 16-bit integer data field; first make a bitmap of the proper size and shape
	int w = amAsset->getWidth();
	int h = amAsset->getHeight();
	float* data = (float*)amAsset->data();

	FIBITMAP* fibm = FreeImage_AllocateT(FIT_INT16, w, h);
	if (fibm) {
		memcpy(FreeImage_GetBits(fibm), data, w*h*sizeof(short));
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

		FreeImage_Unload(fibm);
	}
}
