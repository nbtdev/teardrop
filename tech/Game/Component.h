/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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

		virtual bool initialize();
		virtual bool destroy();
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
