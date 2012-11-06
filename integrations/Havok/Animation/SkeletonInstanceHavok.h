/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SKELETONINSTANCEHAVOK_INCLUDED)
#define SKELETONINSTANCEHAVOK_INCLUDED

#include "Animation/SkeletonInstance.h"
class hkaAnimatedSkeleton;

namespace Teardrop
{
	class SkeletonInstanceHavok : public SkeletonInstance
	{
		hkaAnimatedSkeleton* m_pInstance;

	public:
		SkeletonInstanceHavok(Rig* pRig);
		~SkeletonInstanceHavok();

		size_t getBoneIndexByName(const String& name) const;

		hkaAnimatedSkeleton* getAnimatedSkeleton();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SKELETONINSTANCEHAVOK_INCLUDED
