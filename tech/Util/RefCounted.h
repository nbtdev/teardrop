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

#if !defined(REFCOUNTED_INCLUDED)
#define REFCOUNTED_INCLUDED

namespace Teardrop
{
	/*
		RefCounted is the base class for all ref-countable classes
		in the codebase. The ref-count mechanism is the standard one
		used throughout the known universe, and behaves in the same 
		way as typically expected. 
	*/

	class RefCounted
	{
		size_t m_refCount;

	public:
		RefCounted() { m_refCount = 0; }
		virtual ~RefCounted() { release(); }

		//! last-call chance to clean up for derived classes
		virtual void destroy() {}

		//! inc refcount
		void addRef() { ++m_refCount; }

		//! dec refcount (and delete this object if count <= 0)
		void release() 
		{ 
			if (--m_refCount <= 0) 
			{ 
				destroy(); 
				delete this; 
			} 
		}

	private:
		RefCounted(const RefCounted&); // not implemented
		RefCounted& operator=(const RefCounted&); // not implemented
	};
} // namespace Teardrop

#endif // REFCOUNTED_INCLUDED
