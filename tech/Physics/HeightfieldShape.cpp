/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "stdafx.h"
#include "HeightfieldShape.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Memory/Allocators.h"

using namespace CoS;
//---------------------------------------------------------------------------
HeightfieldShape::HeightfieldShape()
{
	m_pData = 0;
}
//---------------------------------------------------------------------------
HeightfieldShape::~HeightfieldShape()
{
}
//---------------------------------------------------------------------------
bool HeightfieldShape::initialize(
	void* pData, // this object makes a copy of this data
	size_t width, 
	size_t height,
	size_t bpp, // bytes/pixel, i.e. 8-bit greyscale = 1 byte/pixel
	const Vector4& scale // world scaling terms (x, y, z)
)
{
	if (!pData)
	{
		return false;
	}

	size_t bufSize = width * height * bpp;
	m_pData = COS_ALLOCATE_ALIGNED(DEFAULT, bufSize, 16);
	memcpy(m_pData, pData, bufSize);

	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_scale = scale;

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShape::destroy()
{
	if (m_pData)	
	{
		COS_DEALLOCATE(DEFAULT, m_pData);
		m_pData = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShape::update(float deltaT)
{
	return true;
}
