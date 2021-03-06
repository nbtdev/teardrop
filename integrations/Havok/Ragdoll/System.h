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

#if !defined(HAVOK_RAGDOLL_SYSTEM_H)
#define HAVOK_RAGDOLL_SYSTEM_H

#include "Ragdoll/RagdollSystem.h"

namespace Teardrop
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
				class System : public Teardrop::RagdollSystem
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

					// Teardrop::RagdollSystem implementation
					Teardrop::RagdollController* createController();
					void destroyController(Teardrop::RagdollController*);
				};
			}
		}
	}
}

#endif // HAVOK_RAGDOLL_SYSTEM_H
