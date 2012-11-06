/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ZONEDATA_INCLUDED)
#define ZONEDATA_INCLUDED

namespace Teardrop
{
	class Zone;

	class ZoneData
		: public Teardrop::Reflection::Object,
		public wxTreeItemData // so it can be attached to wxTreeCtrl nodes
	{
		Teardrop::Zone* m_pZone;

	public:
		TD_CLASS(ZoneData, Object);
		TD_PROPERTY(Name, "Zone Name", String, "(undefined)", 0);
		TD_PROPERTY(Type, "Zone Type", String, "(undefined)", 0);

		ZoneData();
		~ZoneData();

		Teardrop::Zone* getZone();
		void setZone(Teardrop::Zone* pZone);
		String toString() { return getName(); }
	};
}

#endif // ZONEDATA_INCLUDED
