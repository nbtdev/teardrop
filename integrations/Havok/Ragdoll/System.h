/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_RAGDOLL_SYSTEM_H)
#define HAVOK_RAGDOLL_SYSTEM_H

#include "Ragdoll/RagdollSystem.h"

namespace CoS
{
	class Allocator;
}

namespace Teardrop
{
	namespace Integration
	{
		namespace Havok
		{
			namespace Ragdoll
			{
				class System : public CoS::RagdollSystem
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

					// CoS::RagdollSystem implementation
					CoS::RagdollController* createController();
					void destroyController(CoS::RagdollController*);
				};
			}
		}
	}
}

#endif // HAVOK_RAGDOLL_SYSTEM_H
