/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ANIMATION_INCLUDED)
#define ANIMATION_INCLUDED

#include "Serialization/Serialization.h"
#include "Serialization/SerialPointer.h"
#include "Resource/Resource.h"
#include "Memory/Memory.h"
#include "Util/System.h"

namespace Teardrop
{
	class Stream;
	class ResourceSerializer;
	class AnimationBlender;
	class SkeletonInstance;

	class Animation : public Resource
	{
	public:
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

		TD_DECLARE_ALLOCATOR();

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
