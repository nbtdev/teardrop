/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "StaticMeshScene.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(StaticMeshScene);

StaticMeshScene::StaticMeshScene()
	: mAsset(0)
{
}

StaticMeshScene::StaticMeshScene(StaticMeshAsset* meshAsset)
	: mAsset(meshAsset)
{
}

StaticMeshScene::~StaticMeshScene()
{
}

StaticMeshAsset* StaticMeshScene::asset()
{
	return mAsset;
}
