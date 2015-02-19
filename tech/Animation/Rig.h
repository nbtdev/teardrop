/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RIG_INCLUDED)
#define RIG_INCLUDED

#include "Serialization/Serialization.h"
#include "Serialization/SerialPointer.h"
#include "Resource/Resource.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Bone;
	class SkeletonInstance;
	class Stream;
	class ResourceSerializer;

	class Rig : public Resource
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

		//! loads rig data from stream; owns the data once loaded
		bool load(Stream& stream);
		//! release the resource when done with it
		bool release();

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

#endif // RIG_INCLUDED
