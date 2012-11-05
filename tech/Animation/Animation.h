/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(ANIMATION_INCLUDED)
#define ANIMATION_INCLUDED

#include "Serialization/Serialization.h"
#include "Serialization/SerialPointer.h"
#include "Resource/Resource.h"
#include "Memory/Memory.h"
#include "Util/System.h"

namespace CoS
{
	class Stream;
	class ResourceSerializer;
	class AnimationBlender;
	class SkeletonInstance;
	struct FourCC;

	class Animation : public Resource
	{
		DECLARE_SERIALIZABLE(Animation);
		DECLARE_SERIALIZABLE_VTABLE

	public:
		static const FourCC& RESOURCE_TYPE;

		//! normal c'tor (cannot fail)
		Animation();
		//! placement c'tor (cannot fail)
		Animation(int);
		//! d'tor (cannot fail)
		virtual ~Animation();

		virtual bool initialize(
			void* pData,
			unsigned int dataLen
			);

		virtual bool destroy();

		//! loads rig data from stream; owns the data once loaded
		bool load(Stream& stream);
		//! release the resource when done with it
		bool release();

		/**
			Serialization
		*/
		//! package for storage
		virtual bool serialize(ResourceSerializer& serializer);

		COS_DECLARE_ALLOCATOR();

	protected:
		SerialPointer<void> m_pData;
		unsigned int m_dataLen;
	};

	class AnimationSystem : public Teardrop::System
	{
	public:
		//! create an animation blender
		virtual AnimationBlender* createBlender() = 0;
	};
}

#endif // ANIMATION_INCLUDED
