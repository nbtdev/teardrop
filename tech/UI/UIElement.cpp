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
#include "UIElement.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
Element::Element()
{
	m_bAbsPos = false;
	m_bAbsSize = false;
	m_hAlign = HALIGN_NOTSET;
	m_vAlign = VALIGN_NOTSET;
	m_pNotify = 0;
	m_bVisible = true; // default to visible
	m_bProcessInput = true; // default to input processing
}
//---------------------------------------------------------------------------
Element::~Element()
{
}
//---------------------------------------------------------------------------
void Element::getSize(float& width, float& height, bool& abs) const
{
	abs = (m_bAbsSize != 0);
	width = m_size.x;
	height = m_size.y;
}
//---------------------------------------------------------------------------
const Vector2& Element::getSize(bool& abs) const
{
	abs = (m_bAbsSize != 0);
	return m_size;
}
//---------------------------------------------------------------------------
void Element::getPosition(float& x, float& y, bool& abs) const 
{
	abs = (m_bAbsPos != 0);
	x = m_position.x;
	y = m_position.y;
}
//---------------------------------------------------------------------------
const Vector2& Element::getPosition(bool& abs) const
{
	abs = (m_bAbsPos != 0);
	return m_position;
}
//---------------------------------------------------------------------------
void Element::setSize(float width, float height, bool abs)
{
	m_size.x = width;
	m_size.y = height;
	m_bAbsSize = abs ? 1 : 0;
}
//---------------------------------------------------------------------------
void Element::setSize(const Vector2& size, bool abs)
{
	m_size = size;
	m_bAbsSize = abs ? 1 : 0;
}
//---------------------------------------------------------------------------
void Element::setPosition(float x, float y, bool abs)
{
	m_position.x = x;
	m_position.y = y;
	m_bAbsPos = abs ? 1 : 0;
}
//---------------------------------------------------------------------------
void Element::setPosition(const Vector2& pos, bool abs)
{
	m_position = pos;
	m_bAbsPos = abs ? 1 : 0;
}
//---------------------------------------------------------------------------
bool Element::update(float deltaT)
{
	return true;
}
//---------------------------------------------------------------------------
bool Element::render(Renderer* pRenderer)
{
	return true;
}
//---------------------------------------------------------------------------
void Element::setAlignment(HAlign hAlign, VAlign vAlign)
{
	m_hAlign = hAlign;
	m_vAlign = vAlign;
}
//---------------------------------------------------------------------------
void Element::setNotificationHandler(IElementNotifications* pHandler)
{
	m_pNotify = pHandler;
}
//---------------------------------------------------------------------------
void Element::onResize(float width, float height)
{
	// figure out new pixel position and size based on new width and height
	float x = m_position.x, y = m_position.y;
	float sx = m_size.x;
	float sy = m_size.y;

	// use the low likelihood of anyone creating a UI element of less than 
	// one pixel size in either dimension, as a flag for absolute vs 
	// relative sizing (> 1 means absolute, less means relative/normalized)
	if (sx > 1)
		sx /= width;
	if (sy > 1)
		sy /= height;

	switch (m_hAlign)
	{
	case HALIGN_LEFT:
		x = 0;
		break;

	case HALIGN_CENTER:
		x = (1 - sx) * 0.5f;
		break;

	case HALIGN_RIGHT:
		x = 1 - sx;
		break;

	case HALIGN_NOTSET:
	default:
		break;
	}

	switch (m_vAlign)
	{
	case VALIGN_TOP:
		y = 0;
		break;

	case VALIGN_MIDDLE:
		y = (1 - sy) * 0.5f;
		break;

	case VALIGN_BOTTOM:
		y = 1 - sy;
		break;

	case HALIGN_NOTSET:
	default:
		break;
	}

	m_position.x = x;
	m_position.y = y;
	m_positionCache.x = x * width;
	m_positionCache.y = y * height;

	Vector4 pos(
		MathUtil::floor(m_positionCache.x) - 0.5f, 
		MathUtil::floor(m_positionCache.y) - 0.5f, 
		0, 
		0);

	Vector4 scale(sx * width, sy * height, 1, 0);

	m_inst.setTransform(Transform(pos, Quaternion::IDENTITY, scale));
}
