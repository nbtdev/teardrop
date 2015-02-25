/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "HeightfieldAssetMetadata.h"
#include "Asset/HeightfieldAsset.h"
#include "FreeImage.h"
#include <cstring>

using namespace std::placeholders;
using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(HeightfieldAssetMetadata);

HeightfieldAssetMetadata::HeightfieldAssetMetadata()
{
}

HeightfieldAssetMetadata::HeightfieldAssetMetadata(HeightfieldAsset* asset)
{
	mObject = asset;
    PropertyChanged.bind(std::bind(&HeightfieldAssetMetadata::onPropertyChanged, this, _1));
}

HeightfieldAssetMetadata::~HeightfieldAssetMetadata()
{
    PropertyChanged.unbind(std::bind(&HeightfieldAssetMetadata::onPropertyChanged, this, _1));
}

void HeightfieldAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}

void HeightfieldAssetMetadata::generateThumbnail()
{
	if (!mObject) 
		return;

	HeightfieldAsset* hfAsset = static_cast<HeightfieldAsset*>(mObject);

	// what we have is a single-channel 32-bit floating point data field; first make a bitmap of the proper size and shape
	int w = hfAsset->getWidth();
	int h = hfAsset->getHeight();
	float* data = (float*)hfAsset->data();

	FIBITMAP* fibm = FreeImage_AllocateT(FIT_FLOAT, w, h);
	if (fibm) {
		memcpy(FreeImage_GetBits(fibm), data, w*h*sizeof(float));
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
