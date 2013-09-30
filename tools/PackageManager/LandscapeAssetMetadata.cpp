/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "LandscapeAssetMetadata.h"
#include "Asset/LandscapeAsset.h"
#include "FreeImage.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(LandscapeAssetMetadata);

LandscapeAssetMetadata::LandscapeAssetMetadata()
{
}

LandscapeAssetMetadata::LandscapeAssetMetadata(LandscapeAsset* asset)
{
	mObject = asset;
}

LandscapeAssetMetadata::~LandscapeAssetMetadata()
{
}

void LandscapeAssetMetadata::generateThumbnail()
{
}