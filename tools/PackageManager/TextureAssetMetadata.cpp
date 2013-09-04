/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TextureAssetMetadata.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(TextureAssetMetadata);

TextureAssetMetadata::TextureAssetMetadata()
	: mTexAsset(0)
{
}

TextureAssetMetadata::TextureAssetMetadata(TextureAsset* asset)
	: mTexAsset(asset)
{
	PropertyChanged.bind(fastdelegate::MakeDelegate(this, &TextureAssetMetadata::onPropertyChanged));
}

TextureAssetMetadata::~TextureAssetMetadata()
{
	PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &TextureAssetMetadata::onPropertyChanged));
}

void TextureAssetMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}