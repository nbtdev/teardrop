/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "HeightfieldAssetMetadata.h"
#include "Asset/HeightfieldAsset.h"
#include "FreeImage.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(HeightfieldAssetMetadata);

HeightfieldAssetMetadata::HeightfieldAssetMetadata()
	: mHFAsset(0)
{
}

HeightfieldAssetMetadata::HeightfieldAssetMetadata(HeightfieldAsset* asset)
	: mHFAsset(asset)
{
	PropertyChanged.bind(fastdelegate::MakeDelegate(this, &HeightfieldAssetMetadata::onPropertyChanged));
}

HeightfieldAssetMetadata::~HeightfieldAssetMetadata()
{
	PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &HeightfieldAssetMetadata::onPropertyChanged));
}

void HeightfieldAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}

struct RGB { unsigned char r, g, b; };

void HeightfieldAssetMetadata::generateThumbnail()
{
	if (!mHFAsset) 
		return;

	// what we have is a single-channel 32-bit floating point data field; first make a bitmap of the proper size and shape
	int w = mHFAsset->getWidth();
	int h = mHFAsset->getHeight();
	float* data = (float*)mHFAsset->data();

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