/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LOADLISTENER_INCLUDED)
#define LOADLISTENER_INCLUDED

namespace Teardrop
{
	class Scene;
	class Zone;
	class ZoneObject;

	class LoadListener
	{
	public:
		virtual void onSceneLoadBegin(Teardrop::Scene* pScene) = 0;
		virtual void onZoneCreated(Teardrop::Zone* pZone) = 0;
		virtual void onZoneObjectCreated(Teardrop::ZoneObject* pObject) = 0;
		virtual void onZoneObjectAdded(Teardrop::Zone* pZone, Teardrop::ZoneObject* pObject) = 0;
		virtual void onSceneLoadEnd(Teardrop::Scene* pScene) = 0;
	};
}

#endif // LOADLISTENER_INCLUDED
