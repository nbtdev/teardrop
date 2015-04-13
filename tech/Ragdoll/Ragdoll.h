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
