/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ANIMATIONHAVOK_INCLUDED)
#define ANIMATIONHAVOK_INCLUDED

#include "Serialization/SerialPointerArray.h"
#include "Animation/Animation.h"

class hkaAnimation;
class hkaAnimationBinding;
class hkaAnimationControl;
class hkResource;

namespace Teardrop
{
	class AnimationHavok : public Animation
	{
		DECLARE_SERIALIZABLE(AnimationHavok);

		SerialPointer<hkaAnimationBinding> m_pBinding;
		SerialPointer<hkResource> m_pResource;
		SerialPointerArray<hkaAnimationControl> m_controls;

		void delayInit();

	public:
		//! normal c'tor (cannot fail)
		AnimationHavok();
		//! placement c'tor (cannot fail)
		AnimationHavok(int);
		//! d'tor (cannot fail)
		virtual ~AnimationHavok();

		bool initialize(
			void* pData,
			unsigned int dataLen
			);

		bool destroy();
		bool release();

		// create an animation control, returns a reference to it
		size_t createControl();
		// obtain a pointer to the referenced animation control
		hkaAnimationControl* getControl(size_t idx);

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

		TD_DECLARE_ALLOCATOR();

		enum AnimationControlHandleType 
		{
			INVALID_ANIMATION_CONTROL_HANDLE = size_t(-1),
		};
	};
}

#endif // ANIMATIONHAVOK_INCLUDED
