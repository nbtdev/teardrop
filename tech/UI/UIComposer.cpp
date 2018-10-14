/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

//#include "stdafx.h"
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
