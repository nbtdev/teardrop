/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AttributeMapAssetMetadata.h"
#include "Asset/AttributeMapAsset.h"
#include "FreeImage.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(AttributeMapAssetMetadata);

AttributeMapAssetMetadata::AttributeMapAssetMetadata()
	: mAMAsset(0)
{
}

AttributeMapAssetMetadata::AttributeMapAssetMetadata(AttributeMapAsset* asset)
	: mAMAsset(asset)
{
	PropertyChanged.bind(fastdelegate::MakeDelegate(this, &AttributeMapAssetMetadata::onPropertyChanged));
}

AttributeMapAssetMetadata::~AttributeMapAssetMetadata()
{
	PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &AttributeMapAssetMetadata::onPropertyChanged));
}

void AttributeMapAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}

void AttributeMapAssetMetadata::generateThumbnail()
{
	if (!mAMAsset) 
		return;

	// what we have is a two-channel 16-bit integer data field; first make a bitmap of the proper size and shape
	int w = mAMAsset->getWidth();
	int h = mAMAsset->getHeight();
	float* data = (float*)mAMAsset->data();

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