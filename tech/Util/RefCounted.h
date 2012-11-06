/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
