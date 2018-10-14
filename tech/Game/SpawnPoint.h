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
