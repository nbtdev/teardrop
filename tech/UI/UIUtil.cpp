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
#include "UIDefs.h"
#include "UIUtil.h"
#include "UILayoutTarget.h"
#include "Basic/UIQuadRenderer.h"
#include "Basic/UIQuadElement.h"
#include "SWF/UIFlashRenderer.h"
#include "SWF/UIFlashElement.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxTexture.h"
#include "Resource/ResourceHandle.h"
#include "Resource/ResourceManager.h"
#include "Memory/Memory.h"
#include "Util/Environment.h"
#include "Util/StringUtil.h"
#include "Util/_String.h"
#include "tinyxml.h"

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
Renderer* Util::createQuadRenderer(GfxRenderer* pRenderer)
{
	QuadRenderer* pRend = TD_NEW QuadRenderer();
	pRend->initialize(pRenderer);
	return pRend;
}
//---------------------------------------------------------------------------
Renderer* Util::createFlashRenderer(GfxRenderer* pRenderer)
{
	FlashRenderer* pRend = TD_NEW FlashRenderer();
	pRend->initialize(pRenderer);
	return pRend;
}
//---------------------------------------------------------------------------
Element* Util::createElement(const ElementParams& params)
{
	Element* pRtn = 0;

	if (params.type == ET_BASIC)
	{
		// basic quad element
		HResource hTex = INVALID_RESOURCE_HANDLE;

		if (params.textureName)
		{
			hTex = Environment::get().pResourceMgr->acquire(
				GfxTexture::RESOURCE_TYPE, params.textureName);
		}

		// obtain a texture resource handle by 
		QuadElement* pElem = TD_NEW QuadElement;
		if (!pElem->initialize(hTex, params))
		{
			delete pElem;
			return 0;
		}

		pRtn = pElem;
	}

	if (params.type == ET_FLASH)
	{
		// SWF (Flash) element
		HResource hTex = INVALID_RESOURCE_HANDLE;

		if (params.textureName)
		{
			hTex = Environment::get().pResourceMgr->acquire(
				GfxTexture::RESOURCE_TYPE, params.textureName);
		}

		// obtain a texture resource handle by 
		FlashElement* pElem = TD_NEW FlashElement;
		if (!pElem->initialize(params.resourceName, hTex, params))
		{
			delete pElem;
			return 0;
		}

		pRtn = pElem;
	}

	pRtn->setAlignment(params.hAlign, params.vAlign);
	pRtn->setSize(params.w, params.h);
	pRtn->setPosition(params.x, params.y);

	return pRtn;
}
//---------------------------------------------------------------------------
void Util::destroyUIRenderer(Renderer* pRenderer)
{
	if (pRenderer)
	{
		pRenderer->destroy();
		delete pRenderer;
	}
}
//---------------------------------------------------------------------------
void Util::destroyElement(Element* pElem)
{
	delete pElem;
}
//---------------------------------------------------------------------------
bool Util::layout(TiXmlElement& root, LayoutTarget& target, const char* path)
{
	// one layout per definition
	TiXmlElement* pLayout = root.FirstChildElement("layout");
	if (!pLayout)
		return false;

	TiXmlElement* pLayer = pLayout->FirstChildElement("layer");
	while (pLayer)
	{
		int zOrder = -1;
		const char* pZorder = pLayer->Attribute("zorder");
		if (pZorder)
			StringUtil::fromString(pZorder, zOrder);

		target.addLayer(zOrder);

		// get all elements for this layer
		TiXmlElement* pElem = pLayer->FirstChildElement("element");
		while (pElem)
		{
			ElementParams params;

			params.hAlign = HALIGN_NOTSET;
			params.vAlign = VALIGN_NOTSET;
			params.elementName = 0;
			params.resourceName = 0;
			params.textureName = 0;
			params.x = 0;
			params.y = 0;
			params.w = 0;
			params.h = 0;
			params.type = ET_BASIC;

			const char* attr = pElem->Attribute("type");
			if (attr)
			{
				// anything but "swf" will be treated as "basic"
				if (_stricmp(attr, "swf") == 0)
					params.type = ET_FLASH;
			}

			attr = pElem->Attribute("name");
			if (attr)
			{
				params.elementName = attr;
			}

			attr = pElem->Attribute("halign");
			if (attr)
			{
				// anything but these will be treated as "not set"
				if (_stricmp(attr, "left") == 0)
					params.hAlign = HALIGN_LEFT;
				if (_stricmp(attr, "center") == 0)
					params.hAlign = HALIGN_CENTER;
				if (_stricmp(attr, "right") == 0)
					params.hAlign = HALIGN_RIGHT;
			}

			attr = pElem->Attribute("valign");
			if (attr)
			{
				// anything but these will be treated as "not set"
				if (_stricmp(attr, "top") == 0)
					params.vAlign = VALIGN_TOP;
				if (_stricmp(attr, "middle") == 0)
					params.vAlign = VALIGN_MIDDLE;
				if (_stricmp(attr, "bottom") == 0)
					params.vAlign = VALIGN_BOTTOM;
			}

			attr = pElem->Attribute("x");
			if (attr)
			{
				StringUtil::fromString(attr, params.x);
			}

			attr = pElem->Attribute("y");
			if (attr)
			{
				StringUtil::fromString(attr, params.y);
			}

			attr = pElem->Attribute("w");
			if (attr)
			{
				StringUtil::fromString(attr, params.w);
			}

			attr = pElem->Attribute("h");
			if (attr)
			{
				StringUtil::fromString(attr, params.h);
			}

			attr = pElem->Attribute("u");
			if (attr)
			{
				StringUtil::fromString(attr, params.u);
			}

			attr = pElem->Attribute("v");
			if (attr)
			{
				StringUtil::fromString(attr, params.v);
			}

			const char* pSource = pElem->GetText();
			String source(path);

			if (pSource)
			{
				source += pSource;
				params.resourceName = source;
			}

			// notify the caller
			target.addElement(params);

			pElem = pElem->NextSiblingElement("element");
		}

		pLayer = pLayer->NextSiblingElement("layer");
	}

	return true;
}
