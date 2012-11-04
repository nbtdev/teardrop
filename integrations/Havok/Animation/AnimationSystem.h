/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_ANIMATION_SYSTEM_H)
#define HAVOK_ANIMATION_SYSTEM_H

#include "Util/System.h"

namespace CoS
{
	class AnimationBlender;
}

namespace Teardrop
{
	namespace Integration
	{
		namespace Havok
		{
			class System : public Teardrop::System
			{
				CoS::Allocator* m_pAllocator;

			public:
				System();
				~System();

				// Teardrop::System implementation
				void initialize();
				void shutdown();
				void getTypes(Type* typeArray, int& typeCount);
				void setAllocator(CoS::Allocator* pAllocator);
				CoS::Allocator* getAllocator();

				CoS::AnimationBlender* createBlender();
			};
		}
	}
}

#endif // HAVOK_ANIMATION_SYSTEM_H