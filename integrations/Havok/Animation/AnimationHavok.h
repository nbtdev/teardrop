/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
