/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SKELETONINSTANCE_INCLUDED)
#define SKELETONINSTANCE_INCLUDED
#include "Memory/Memory.h"

namespace Teardrop
{
	class Rig;
	class Matrix44;
	class String;

	class SkeletonInstance
	{
	public:
		//! normal c'tor (cannot fail)
		SkeletonInstance(Rig* pRig);
		//! d'tor (cannot fail)
		virtual ~SkeletonInstance();

		size_t getBoneCount() const;
		const Matrix44* getBoneCache() const; // read-only
		Matrix44* getBoneCache(); // read-write
		virtual size_t getBoneIndexByName(const String& name) const;

		TD_DECLARE_ALLOCATOR();

	protected:
		Matrix44* m_pBoneCache;
		size_t m_boneCount;
	};
}

#endif // SKELETONINSTANCE_INCLUDED
