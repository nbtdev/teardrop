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

#include "TextureAsset.h"
#include "Stream/Stream.h"

using namespace Teardrop;

TD_CLASS_IMPL(TextureAsset);
TD_ENUM(TextureAsset, Type, "Texture Type");
TD_ENUM_VALUE(TextureAsset, Type, TEXTYPE_1D, "1-dimensional texture");
TD_ENUM_VALUE(TextureAsset, Type, TEXTYPE_2D, "2-dimensional texture");
TD_ENUM_VALUE(TextureAsset, Type, TEXTYPE_3D, "3-dimensional texture");
TD_ENUM(TextureAsset, Format, "Texture Format");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_R, "1-channel, 1 unsigned byte per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_RG, "2-channel, 1 unsigned byte per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_RGB, "3-channel, 1 unsigned byte per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_ARGB, "4-channel, 1 unsigned byte per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_R_F32, "1-channel, 1 single-precision float per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_RG_F32, "2-channel, 1 single-precision float per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_RGB_F32, "3-channel, 1 single-precision float per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_ARGB_F32, "4-channel, 1 single-precision float per channel");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_BC1, "4-channel, block-compressed, 1-bit alpha");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_BC2, "4-channel, block-compressed, 4-bit alpha");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_BC3, "4-channel, block-compressed, 4-bit alpha");
TD_ENUM_VALUE(TextureAsset, Format, TEXFMT_PVRTC, "4-channel, block-compressed, PowerVR only");

TextureAsset::TextureAsset()
{
}

TextureAsset::~TextureAsset()
{
}

uint64_t TextureAsset::serialize(Stream& strm)
{
    uint64_t len = mLength;
	strm.write(&len, sizeof(len));

	if (strm.write(mData, len))
		return len + sizeof(len);
	else
		return 0;
}

uint64_t TextureAsset::deserialize(Stream& strm)
{
	delete mDynamicData;
	mDynamicData = 0;

    uint64_t len = 0;
	if (strm.read(&len, sizeof(len))) {
		mDynamicData = TD_NEW unsigned char[len];
		mData = mDynamicData;
		mLength = len;

		if (strm.read(mData, mLength))
			len += sizeof(len);
	}

	return len;
}
