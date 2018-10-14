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
