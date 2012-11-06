/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BONEHAVOK_INCLUDED)
#define BONEHAVOK_INCLUDED

#include "Animation/Bone.h"

class hkaBone;
class hkaSkeleton;

namespace Teardrop
{
	class BoneHavok : public Bone
	{
		hkaBone* m_pBone;
		hkaSkeleton* m_pParent;

	public:
		BoneHavok();
		~BoneHavok();

		bool initialize(hkaSkeleton* pParent, hkaBone* pBone);
		bool destroy();

		const char* getName() const;
		Bone* getParent(const char* name) const;
		size_t getNumChildren() const;
		const Bone* getChild(size_t index) const;
		Bone* getChild(size_t index);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // BONEHAVOK_INCLUDED
