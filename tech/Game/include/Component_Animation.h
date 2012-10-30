/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COMPONENT_ANIMATION_INCLUDED)
#define COMPONENT_ANIMATION_INCLUDED

#include "Resource/include/ResourceHandle.h"
#include <map>
#include "Game/include/Component.h"
#include "Memory/include/Allocators.h"

namespace CoS
{
	struct Environment;
	struct FourCC;
	class ZoneObject;
	class AnimationBlender;
	class SkeletonInstance;
	class Matrix44;
	class Quaternion;

	class AnimationComponent
		: public Component
	{
		HResource m_hRig;
		typedef std::map<String, HResource> AnimationLUT;
		AnimationLUT m_lut;
		AnimationBlender* m_pBlender;

	public:
		COS_CLASS(AnimationComponent, Component);
		COS_CLASS_CREATABLE();

		COS_PROPERTY(RigName, "Name of rig (skeleton) asset file to use", String, "(undefined)", FileChooser);
		COS_PROPERTY(BlendConfigName, "Names of animation blend tree config file to use", String, "", 0);

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

		COS_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // COMPONENT_ANIMATION_INCLUDED
