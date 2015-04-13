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

#if !defined(SHAREDPOINTER_INCLUDED)
#define SHAREDPOINTER_INCLUDED

#include <assert.h>
#include "Memory/Memory.h"

namespace Teardrop
{
	/** non-intrusive ref-counted object wrapper
	*/
	template <class T>
	class SharedPointer
	{
		size_t* m_pRefCount;

	protected:
		T* m_pObj;

		void release()
		{
			if (m_pRefCount)
			{
				if (--*m_pRefCount <= 0)
				{
					delete m_pObj;
					GetDEFAULTAllocator()->Deallocate(m_pRefCount);
					m_pObj = 0;
					m_pRefCount = 0;
				}
			}
		}

	public:
		//!< default c'tor, does not init (use bind() after this)
		SharedPointer()
		{
			m_pRefCount = 0;
			m_pObj = 0;
		}

		virtual ~SharedPointer()
		{
			release();
		}

		//!< c'tor takes raw pointer as param
		template<class _T>
		explicit SharedPointer(const _T* pObj)
		{
			m_pRefCount = (size_t*)GetDEFAULTAllocator()->Allocate(sizeof(size_t) TD_ALLOC_SITE);
			*m_pRefCount = 1;
			m_pObj = const_cast<_T*>(pObj); // the compiler will check T vs. _T for us
		}

		//!< copy c'tor
		SharedPointer(const SharedPointer& other)
		{
			m_pRefCount = other.m_pRefCount;
			m_pObj = other.m_pObj;

			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
		}

		//!< copy c'tor
		template<typename Y>
		SharedPointer(SharedPointer<Y>& other)
		{
			m_pObj = static_cast<T*>(other.get());
			m_pRefCount = other.getRefCountPtr();
			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
		}

		//!< bind this shared pointer to a raw pointer (only if not already bound)
		void bind(T* pObj)
		{
			assert(!m_pObj);
			assert(!m_pRefCount);
			if (m_pObj || m_pRefCount)
			{
				return;
			}

			*this = SharedPointer<T>(pObj);
		}

		//!< (member) assignment operator
		template<typename Y>
		SharedPointer& operator=(SharedPointer<Y>& other)
		{
			release();
			m_pObj = static_cast<T*>(other.m_pObj);
			m_pRefCount = other.m_pRefCount;
			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
			return *this;
		}

		//!< (member) assignment operator
		SharedPointer& operator=(const SharedPointer& other)
		{
			release();
			m_pObj = static_cast<T*>(other.m_pObj);
			m_pRefCount = other.m_pRefCount;
			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
			return *this;
		}

		//!< (member) assignment operator (from raw pointer)
		SharedPointer& operator=(const T* other)
		{
			*this = SharedPointer<T>(other);
			return *this;
		}

		//! de-reference operator
		virtual T* operator->()
		{
			return m_pObj;
		}

		//! de-reference operator
		virtual const T* operator->() const
		{
			return m_pObj;
		}

		//!< (member) cast operator
		virtual operator T*()
		{
			return m_pObj;
		}

		//!< (member) unary negation operator
		int operator!()
		{
			return (int)!m_pObj;
		}

		//!< get raw pointer value (no ref count increment)
		T* get()
		{
			return m_pObj;
		}

		//!< get const raw pointer value (no ref count increment)
		const T* get() const
		{
			return m_pObj;
		}

		//! null this pointer, releasing it 
		void setNull()
		{
			release();
			m_pObj = 0;
		}

		//! add a reference to this object
		void addRef()
		{
			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
		}

		//! release a refcount on this object
		void releaseRef()
		{
			release();
		}

		//!< get current refcount value (convenience for = operator)
		size_t getRefCount()
		{
			if (m_pRefCount)
			{
				return *m_pRefCount;
			}

			return 0;
		}

		//! return state of internal pointer
		bool isNull()
		{
			return (0 == m_pObj);
		}

		//!< get current refcount pointer (convenience for = operator)
		size_t* getRefCountPtr()
		{
			return m_pRefCount;
		}
	};
}

#endif // SHAREDPOINTER_INCLUDED
