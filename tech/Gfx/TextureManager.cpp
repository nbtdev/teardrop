/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "TextureManager.h"
#include "Texture2D.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

TextureManager* Singleton<TextureManager>::mInst = 0;

TextureManager::TextureManager()
{
	assert(!mInst);
	mInst = this;
}

TextureManager::~TextureManager()
{

}

