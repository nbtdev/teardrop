/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
