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

#include "Prop.h"
#include "Component_Render.h"
#include "Component_BasicPhysics.h"
#include "Physics/Body.h"
#include "Util/StringUtil.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Prop);
//---------------------------------------------------------------------------
Prop::Prop()
{
	m_bDynamic = true;
}
//---------------------------------------------------------------------------
Prop::~Prop()
{
}
//---------------------------------------------------------------------------
bool Prop::initialize()
{
	return ZoneObject::initialize();
}
//---------------------------------------------------------------------------
bool Prop::destroy()
{
	setObjectId(0);
	return ZoneObject::destroy();
}
//---------------------------------------------------------------------------
bool Prop::update(float deltaT)
{
	RenderComponent* pRend;
	if (findComponents(
		RenderComponent::getClassDef(), (Component**)&pRend))
	{
		pRend->update(deltaT);
	}

	PhysicsComponent* pPhy;
	if (findComponents(
		BasicPhysicsComponent::getClassDef(), (Component**)&pPhy))
	{
		if (!getDynamic() && m_bNeedsTransformUpdate)
		{
			pPhy->setWorldTransform(getTransform());
		}

		pPhy->update(deltaT);
	}

	ZoneObject::update(deltaT);

	return true;
}
