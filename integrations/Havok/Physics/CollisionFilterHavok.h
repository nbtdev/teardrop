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

#if !defined(COLLISIONFILTERHAVOK_INCLUDED)
#define COLLISIONFILTERHAVOK_INCLUDED

#include "Physics/CollisionFilter.h"

class hkpGroupFilter;

namespace Teardrop
{
	class CollisionFilterHavok
		: public CollisionFilter
	{
		hkpGroupFilter* m_pFilter;

	public:
		CollisionFilterHavok();
		~CollisionFilterHavok();

		void enableCollisionBetween(CollisionGroup group1, CollisionGroup group2);
		void enableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2);
		void disableCollisionBetween(CollisionGroup group1, CollisionGroup group2);
		void disableCollisionBetween(CollisionLayer layer1, CollisionLayer layer2);

		hkpGroupFilter* getGroupFilter() const { return m_pFilter; }

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // COLLISIONFILTERHAVOK_INCLUDED
