/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "UILayer.h"
#include "UIElement.h"

using namespace CoS;
using namespace UI;
//---------------------------------------------------------------------------
Layer::Layer()
{
}
//---------------------------------------------------------------------------
Layer::~Layer()
{
	clear();
}
//---------------------------------------------------------------------------
void Layer::clear()
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		//it->second->release();
		delete it->second;
	}

	m_elements.clear();
}
//---------------------------------------------------------------------------
void Layer::addElement(Element* pElement, int zOrder)
{
	m_elements.insert(Elements::value_type(zOrder, pElement));
//	pElement->addRef();
}
//---------------------------------------------------------------------------
void Layer::removeElement(Element* pElement)
{
	for (Elements::iterator it = m_elements.begin();
		it != m_elements.end(); ++it)
	{
		if (it->second == pElement)
		{
			//it->second->release();
			delete it->second;
			m_elements.erase(it);
			break;
		}
	}
}
//---------------------------------------------------------------------------
Layer::ElementRenderOrder Layer::getRenderOrder() const
{
	return m_renderOrder;
}
//---------------------------------------------------------------------------
void Layer::setRenderOrder(ElementRenderOrder order)
{
	m_renderOrder = order;
}
//---------------------------------------------------------------------------
bool Layer::update(float deltaT)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		pElem->update(deltaT);
	}

	return true;
}
//---------------------------------------------------------------------------
bool Layer::render(Renderer* pRenderer)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		it->second->render(pRenderer);
	}
	return true;
}
//---------------------------------------------------------------------------
void Layer::resize(float width, float height)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		pElem->onResize(width, height);
	}
}
//---------------------------------------------------------------------------
bool Layer::injectMouseMove(int x, int y)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectMouseMove(x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Layer::injectMouseDown(int button, int x, int y)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectMouseDown(button, x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Layer::injectMouseUp(int button, int x, int y)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectMouseUp(button, x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Layer::injectKeyDown(int keyCode, int keyChar)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectKeyDown(keyCode, keyChar))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Layer::injectKeyUp(int keyCode, int keyChar)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectKeyUp(keyCode, keyChar))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Layer::injectKeyEvent(size_t msg, size_t wParam, size_t lParam)
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
	{
		Element* pElem = it->second;
		if (pElem->injectKeyEvent(msg, wParam, lParam))
			return true;
	}

	return false;
}
