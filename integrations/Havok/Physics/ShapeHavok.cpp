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
#include "ShapeHavok.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ShapeHavok::ShapeHavok()
{
	m_pShape = 0;
}
//---------------------------------------------------------------------------
ShapeHavok::~ShapeHavok()
{
}
//---------------------------------------------------------------------------
bool ShapeHavok::initialize()
{
	release();
	return Shape::initialize();
}
//---------------------------------------------------------------------------
bool ShapeHavok::release()
{
	if (m_pShape)
	{
		m_pShape->removeReference();
		m_pShape = 0;
	}

	return Shape::release();
}
//---------------------------------------------------------------------------
bool ShapeHavok::update(float deltaT)
{
	return Shape::update(deltaT);
}
//---------------------------------------------------------------------------
hkpShape* ShapeHavok::getHavokShape()
{
	return m_pShape;
}
