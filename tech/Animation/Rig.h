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

#if !defined(RIG_INCLUDED)
#define RIG_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	class Bone;
	class SkeletonInstance;

    class Rig
	{
	public:
		enum SkeletonInstanceHandleType
		{
			INVALID_SKELETON_INSTANCE_HANDLE = size_t(-1),
		};

		//! normal c'tor (cannot fail)
		Rig();
		//! placement c'tor (cannot fail)
		Rig(int);
		//! d'tor (cannot fail)
		virtual ~Rig();

		virtual bool initialize(
			void* pData,
			unsigned int dataLen
			);

		virtual bool destroy();

		virtual Bone* getBone(const char* name);
		virtual Bone* getBone(size_t index);
		virtual size_t getBoneCount();
		virtual size_t createInstance();
		virtual SkeletonInstance* getInstance(size_t idx);

		TD_DECLARE_ALLOCATOR();

	protected:
        void* m_pData;
		unsigned int m_dataLen;
	};
}

#endif // RIG_INCLUDED
