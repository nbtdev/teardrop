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

#include "Component.h"
#include "Game/ComponentHost.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Component);
//---------------------------------------------------------------------------
Component::Component()
{
	m_pHost = 0;
}
//---------------------------------------------------------------------------
Component::~Component()
{
}
//---------------------------------------------------------------------------
void Component::setHost(ComponentHost* pHost)
{
	m_pHost = pHost;
}
//---------------------------------------------------------------------------
bool Component::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Component::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
Reflection::Object* Component::clone() const
{
	// utilize the base functionality 
	Reflection::Object* obj = Reflection::Object::clone();
	Component* comp = static_cast<Component*>(obj);

	// and we have a couple of extra things to copy
	comp->setAssetRootPath(m_assetPath);

	return obj;
}
//---------------------------------------------------------------------------
bool Component::update(float /*deltaT*/)
{
	return true;
}
//---------------------------------------------------------------------------
ComponentHost* Component::getHost() const
{
	return m_pHost;
}
//---------------------------------------------------------------------------
void Component::addedToZone(Zone* /*pZone*/)
{
}
//---------------------------------------------------------------------------
void Component::removeFromZone(Zone* /*pZone*/)
{
}
//---------------------------------------------------------------------------
void Component::setAssetRootPath(const String& rootPath)
{
	m_assetPath = rootPath;
}
//---------------------------------------------------------------------------
const String& Component::getAssetRootPath() const
{
	return m_assetPath;
}
