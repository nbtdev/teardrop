/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SINGLETON_INCLUDED)
#define SINGLETON_INCLUDED

#include <assert.h>

namespace Teardrop
{
	template<typename T>
	class Singleton
	{
	public:
		static T& instance() {
			assert(mInst != 0);
			return *mInst;
		}

		static void shutdown() {
			delete mInst;
			mInst = 0;
		}

	protected:
		Singleton(){}
		virtual ~Singleton(){}

		TD_DECLARE_ALLOCATOR();

		static T* mInst;
	};
}

#endif // SINGLETON_INCLUDED
