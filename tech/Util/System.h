/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(SYSTEM_H_INCLUDED)
#define SYSTEM_H_INCLUDED

namespace Teardrop
{
	class Allocator;

	class System
	{
	public:

		// types of pluggable systems that the runtime knows about
		enum Type
		{
			SYSTEM_ANIMATION,
			SYSTEM_AUDIO,
			SYSTEM_GRAPHICS,
			SYSTEM_PHYSICS,
			SYSTEM_NETWORK,
			SYSTEM_RAGDOLL,
			SYSTEM_SCRIPT,
			SYSTEM_TASK_SCHEDULER,
			SYSTEM_UI,
			SYSTEM_VFX,

			SYSTEM_TYPE_MAX
		};

		virtual void initialize() = 0;
		virtual void shutdown() = 0;
		/** retrieve the system types this implementation supports; pass in the nunmber of elements
		    in the Type array in typeCount, returns number of elements in the populated array
		**/
		virtual void getTypes(/*out*/ Type* typeArray, /*inout*/ int& typeCount) = 0;

		// for custom memory management
		virtual void setAllocator(Allocator* pAllocator) = 0;
		virtual Allocator* getAllocator() = 0;
	};
}

#endif // SYSTEM_H_INCLUDED
