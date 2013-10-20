/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "IndexBuffer.h"

using namespace Teardrop;
using namespace Gfx;

IndexBuffer::IndexBuffer()
	: mSize(0)
	, mCount(0)
{
}

IndexBuffer::~IndexBuffer()
{
}

int IndexBuffer::indexCount()
{
	return mCount;
}

int IndexBuffer::triangleCount()
{
	return mCount / 3;
}

int IndexBuffer::indexSize()
{
	return mSize;
}
