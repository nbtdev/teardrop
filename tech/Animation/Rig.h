/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
