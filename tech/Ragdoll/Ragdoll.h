/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLL_INCLUDED)
#define RAGDOLL_INCLUDED

#include "Resource/Resource.h"
#include "Serialization/SerialPointer.h"
#include "Serialization/Serialization.h"
#include "Memory/Allocators.h"

namespace Teardrop
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

		TD_DECLARE_ALLOCATOR();

	protected:
		SerialPointer<void> m_pData;
		unsigned int m_dataLen;
	};
}

#endif // RAGDOLL_INCLUDED
