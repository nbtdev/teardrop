/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Texture2D.h"

namespace Teardrop {
namespace Gfx {

Texture2D::Texture2D(TextureAsset* asset)
	: mAsset(asset)
{
}

Texture2D::~Texture2D()
{
}

} // Gfx
} // Teardrop
