/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_ANIMATION_SYSTEM_H)
#define HAVOK_ANIMATION_SYSTEM_H

#include "Animation/Animation.h"

namespace Teardrop
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
				class System : public Teardrop::AnimationSystem
				{
					Teardrop::Allocator* m_pAllocator;

				public:
					System();
					~System();

					// Teardrop::System implementation
					void initialize();
					void shutdown();
					void getTypes(Type* typeArray, int& typeCount);
					void setAllocator(Teardrop::Allocator* pAllocator);
					Teardrop::Allocator* getAllocator();

					// Teardrop::AnimationSystem implementation
					Teardrop::AnimationBlender* createBlender();
				};
			}
		}
	}
}

#endif // HAVOK_ANIMATION_SYSTEM_H