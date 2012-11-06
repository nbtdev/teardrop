/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIComposer.h"
#include "UILayer.h"
#include "Memory/Memory.h"

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
Composer::Composer()
{
}
//---------------------------------------------------------------------------
Composer::~Composer()
{
	clear();
}
//---------------------------------------------------------------------------
void Composer::clear()
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		it->second->clear();
		delete it->second;
	}
}
//---------------------------------------------------------------------------
Layer* Composer::addLayer(int zOrder)
{
	Layer* pLayer = TD_NEW Layer;
	m_layers.insert(Layers::value_type(zOrder, pLayer));
	return pLayer;
}
//---------------------------------------------------------------------------
void Composer::removeLayer(Layer* pLayer)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (pLayer == it->second)
		{
			delete it->second;
			m_layers.erase(it);
			break;
		}
	}
}
//---------------------------------------------------------------------------
Composer::LayerRenderOrder Composer::getRenderOrder() const
{
	return m_renderOrder;
}
//---------------------------------------------------------------------------
void Composer::setRenderOrder(LayerRenderOrder order)
{
	m_renderOrder = order;
}
//---------------------------------------------------------------------------
bool Composer::update(float deltaT)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		it->second->update(deltaT);
	}

	return true;
}
//---------------------------------------------------------------------------
bool Composer::render(Renderer* pRenderer)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		it->second->render(pRenderer);
	}

	return true;
}
//---------------------------------------------------------------------------
void Composer::resize(float width, float height)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		it->second->resize(width, height);
	}
}
//---------------------------------------------------------------------------
bool Composer::injectMouseMove(int x, int y)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectMouseMove(x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Composer::injectMouseDown(int button, int x, int y)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectMouseDown(button, x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Composer::injectMouseUp(int button, int x, int y)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectMouseUp(button, x, y))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Composer::injectKeyDown(int keyCode, int keyChar)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectKeyDown(keyCode, keyChar))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Composer::injectKeyUp(int keyCode, int keyChar)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectKeyUp(keyCode, keyChar))
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool Composer::injectKeyEvent(size_t msg, size_t wParam, size_t lParam)
{
	for (Layers::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if (it->second->injectKeyEvent(msg, wParam, lParam))
			return true;
	}

	return false;
}
