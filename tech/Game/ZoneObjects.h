/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ZONEOBJECTS_INCLUDED)
#define ZONEOBJECTS_INCLUDED

#include <vector>

namespace Teardrop
{
	class ZoneObject;

	class ZoneObjects : public std::vector<ZoneObject*> {};
}

#endif // ZONEOBJECTS_INCLUDED
