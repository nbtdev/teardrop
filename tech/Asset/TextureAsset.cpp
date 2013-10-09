/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TextureAsset.h"

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
