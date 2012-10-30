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

#if !defined(RAGDOLL_INCLUDED)
#define RAGDOLL_INCLUDED

#include "Resource/include/Resource.h"
#include "Serialization/include/SerialPointer.h"
#include "Serialization/include/Serialization.h"
#include "Memory/include/Allocators.h"

namespace CoS
{
	class Stream;
	class ResourceSerializer;
	struct FourCC;
	class AnimationBlender;

	class Ragdoll: public Resource
	{
		DECLARE_SERIALIZABLE_VTABLE();
		DECLARE_SERIALIZABLE(Ragdoll);

	public:
		static const FourCC& RESOURCE_TYPE;

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

		//! loads body data from stream; owns the data once loaded
		bool load(Stream& stream);
		//! release the resource when done with it
		bool release();
		bool destroy();

		// make a copy of this object with unique ragdoll
		virtual Ragdoll* clone();

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
}

#endif // RAGDOLL_INCLUDED
