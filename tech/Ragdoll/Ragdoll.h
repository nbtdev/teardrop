/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLL_INCLUDED)
#define RAGDOLL_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class AnimationBlender;

    class Ragdoll
	{
	public:
		//! normal c'tor (cannot fail)
		Ragdoll();
		//! placement c'tor (cannot fail)
		Ragdoll(int);
		//! d'tor (cannot fail)
		virtual ~Ragdoll();

		virtual bool initialize(
			void* pData,
			unsigned int dataLen
			);

		virtual bool initialize();

		virtual bool update(float deltaT, AnimationBlender* pAnimBlender);

		// make a copy of this object with unique ragdoll
		virtual Ragdoll* clone();

		TD_DECLARE_ALLOCATOR();

	protected:
        void* m_pData;
		unsigned int m_dataLen;
	};
}

#endif // RAGDOLL_INCLUDED
