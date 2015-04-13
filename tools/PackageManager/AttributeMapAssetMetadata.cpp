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
