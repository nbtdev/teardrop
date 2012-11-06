/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MEMORYPOOL_INCLUDED)
#define MEMORYPOOL_INCLUDED

#include "Memory/Memory.h"
#include <vector>
#include <stack>

/*
	A template class for a pool allocator
*/

namespace Teardrop
{
	template<typename T>
	class Pool
	{
		typedef std::vector<T> Objects;
		typedef std::stack<T*> Freelist;

		Objects m_objects;
		Freelist m_freelist;

	public:
		Pool() {}
		~Pool() {}

		// initial pool of T objects
		void initialize(int size)
		{
			resize(size);
		}

		// resize pool, potentially deleting all objects in the pool
		void resize(int size)
		{
			if (m_objects.size())
				m_objects.clear();

			m_objects.resize(size);

			// populate the freelist with pointers to these
			for (int i=0; i<size; ++i)
			{
				m_freelist.push(&m_objects[i]);
			}
		}

		// acquire one T instance from the pool
		T* allocate()
		{
			if (!m_freelist.empty())
			{
				T* p = m_freelist.top();
				m_freelist.pop();
				return p;
			}

			return 0;
		}

		// return one T instance to the pool
		void deallocate(T* p)
		{
			// TODO: check to see if this is our object?
			m_freelist.push(p);
		}
	};
}

#endif // MEMORYPOOL_INCLUDED
