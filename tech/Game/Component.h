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

#if !defined(COMPONENT_INCLUDED)
#define COMPONENT_INCLUDED

#include "Reflection/Reflection.h"
#include "Util/_String.h"

class TiXmlElement;

namespace Teardrop
{
	class ZoneObject;
	class Zone;
	class ComponentHost;

	class Component
		: public Reflection::Object
	{
	public:
		TD_CLASS(Component, Object);
		TD_PROPERTY(ServerComponent, "Should this component run on a server?", bool, true, 0);

		Component();
		~Component();

		// Reflection::Object overrides
		bool initialize();
		bool destroy();
		Reflection::Object* clone() const;

		virtual bool update(float deltaT);
		virtual void setHost(ComponentHost* pHost);
		virtual ComponentHost* getHost() const;
		virtual void addedToZone(Zone* pZone);
		virtual void removeFromZone(Zone* pZone);

		void setAssetRootPath(const String& path);
		const String& getAssetRootPath() const;

	protected:
		ComponentHost* m_pHost;
		String m_assetPath;
	};
}

#endif // COMPONENT_INCLUDED
