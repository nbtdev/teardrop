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

#if !defined(COMPONENT_ANIMATION_INCLUDED)
#define COMPONENT_ANIMATION_INCLUDED

#include <map>
#include "Game/Component.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	struct Environment;
    class Animation;
	class AnimationBlender;
	class Matrix44;
	class Quaternion;
    class SkeletonInstance;
    class ZoneObject;

	class AnimationComponent
		: public Component
	{
        typedef std::map<String, Animation*> AnimationLUT;
		AnimationLUT m_lut;
		AnimationBlender* m_pBlender;

	public:
		TD_CLASS(AnimationComponent, Component);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(RigName, "Name of rig (skeleton) asset file to use", String, "(undefined)", FileChooser);
		TD_PROPERTY(BlendConfigName, "Names of animation blend tree config file to use", String, "", 0);

		static const String& SHORT_NAME;
		AnimationComponent();
		virtual ~AnimationComponent();

		bool initialize();
		bool destroy();
		bool addAnimation(const char* name, const char* assetPath);
		bool addRig(const char* assetPath);
		bool update(float deltaT);
		const Matrix44* getBoneCache() const;
		size_t getNumBones() const;
		AnimationBlender* getBlender();

		size_t getBoneIndexByName(const String& name);
		void setAdditionalBoneRotationByIndex(size_t idx, const Quaternion& rot);
		void transitionToState(const String& state);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // COMPONENT_ANIMATION_INCLUDED
