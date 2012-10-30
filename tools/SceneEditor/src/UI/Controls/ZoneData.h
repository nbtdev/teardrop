/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(ZONEDATA_INCLUDED)
#define ZONEDATA_INCLUDED

namespace CoS
{
	class Zone;

	class ZoneData
		: public CoS::Reflection::Object,
		public wxTreeItemData // so it can be attached to wxTreeCtrl nodes
	{
		CoS::Zone* m_pZone;

	public:
		COS_CLASS(ZoneData, Object);
		COS_PROPERTY(Name, "Zone Name", String, "(undefined)", 0);
		COS_PROPERTY(Type, "Zone Type", String, "(undefined)", 0);

		ZoneData();
		~ZoneData();

		CoS::Zone* getZone();
		void setZone(CoS::Zone* pZone);
		String toString() { return getName(); }
	};
}

#endif // ZONEDATA_INCLUDED
