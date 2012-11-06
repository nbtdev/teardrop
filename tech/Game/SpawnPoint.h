/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SPAWNPOINT_INCLUDED)
#define SPAWNPOINT_INCLUDED

#include "Game/ZoneObject.h"

namespace Teardrop
{
	class SpawnPoint
		: public ZoneObject
	{
		size_t m_spawnIndex;
		size_t m_valid;

	public:
		TD_CLASS(SpawnPoint, ZoneObject);
		TD_CLASS_CREATABLE();

		SpawnPoint();
		~SpawnPoint();

		bool initialize();

		bool isValid() const;
		void setValid(bool valid);
		size_t getSpawnIndex() const;
		void setSpawnIndex(size_t idx);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		bool operator==(const SpawnPoint& other);
		bool operator!=(const SpawnPoint& other);
	};
}

#endif // SPAWNPOINT_INCLUDED
