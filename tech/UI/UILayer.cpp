/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UILayer.h"
#include "UIElement.h"

using namespace Teardrop;
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
