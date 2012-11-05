/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_ANIMATION_SYSTEM_H)
#define HAVOK_ANIMATION_SYSTEM_H

#include "Animation/Animation.h"

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
			namespace Animation
			{
				class System : public CoS::AnimationSystem
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

					// CoS::AnimationSystem implementation
					CoS::AnimationBlender* createBlender();
				};
			}
		}
	}
}

#endif // HAVOK_ANIMATION_SYSTEM_H