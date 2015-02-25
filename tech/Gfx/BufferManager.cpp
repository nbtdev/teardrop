/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "BufferManager.h"

using namespace Teardrop;
using namespace Gfx;

template<> BufferManager* Singleton<BufferManager>::mInst = 0;

BufferManager::BufferManager()
{
	assert(!mInst);
	mInst = this;
}

BufferManager::~BufferManager()
{
}
