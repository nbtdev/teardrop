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

#if !defined(RAGDOLLCONTROLLER_INCLUDED)
#define RAGDOLLCONTROLLER_INCLUDED

#include "Resource/ResourceHandle.h"
#include "Memory/Memory.h"

#include "Math/Vector4.h"
#include <vector>
#include <list>

namespace CoS
{
	class AnimationBlender;
	class Transform;
	class World;

	class RagdollController
	{
	public:
		//! normal c'tor (cannot fail)
		RagdollController();
		//! d'tor (cannot fail)
		virtual ~RagdollController();

		virtual bool initialize(HResource hRagdoll);
		virtual bool destroy();
		virtual bool drive(float timeStep, AnimationBlender* pBlender, const Transform& world);
		virtual bool addToWorld(World* pWorld);
		virtual bool removeFromWorld(World* pWorld);

		// debug view of the ragdoll's shapes
		typedef std::vector<Vector4> DisplayGeometry;
		typedef std::list<DisplayGeometry> DisplayGeometries;
		// will return a list of lines (start end start end ...) that describe 
		// the debug view of the ragdoll's geometries
		virtual bool getDisplayGeometry(DisplayGeometries& geom) = 0;

		virtual void setUserData(void*);

		COS_DECLARE_ALLOCATOR();

	protected:
		HResource m_hRagdoll;

	private:
		RagdollController(const RagdollController& other);
		RagdollController& operator=(const RagdollController& other);
	};
}

#endif // RAGDOLLCONTROLLER_INCLUDED
