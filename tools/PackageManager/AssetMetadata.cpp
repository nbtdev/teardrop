/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AssetMetadata.h"
#include "Asset/Asset.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(AssetMetadata);

AssetMetadata::AssetMetadata()
{
}

AssetMetadata::AssetMetadata(Asset* asset)
{
	mObject = asset;
}

AssetMetadata::~AssetMetadata()
{
}
