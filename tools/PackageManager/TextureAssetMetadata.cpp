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

#include "TextureAssetMetadata.h"
#include "Asset/TextureAsset.h"
#include "FreeImage.h"

using namespace std::placeholders;
using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(TextureAssetMetadata);

TextureAssetMetadata::TextureAssetMetadata()
{
}

TextureAssetMetadata::TextureAssetMetadata(TextureAsset* asset)
{
	mObject = asset;
    PropertyChanged.bind(std::bind(&TextureAssetMetadata::onPropertyChanged, this, _1));
}

TextureAssetMetadata::~TextureAssetMetadata()
{
    PropertyChanged.unbind(std::bind(&TextureAssetMetadata::onPropertyChanged, this, _1));
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
