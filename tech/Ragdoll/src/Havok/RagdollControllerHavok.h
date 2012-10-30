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

#if !defined(RAGDOLLCONTROLLERHAVOK_INCLUDED)
#define RAGDOLLCONTROLLERHAVOK_INCLUDED

#include "Ragdoll/include/RagdollController.h"

class hkaRagdollRigidBodyController;

namespace CoS
{
	class RagdollControllerHavok
		: public RagdollController
	{
	public:
		//! normal c'tor (cannot fail)
		RagdollControllerHavok();
		//! d'tor (cannot fail)
		~RagdollControllerHavok();

		bool initialize(HResource hRagdoll);
		bool destroy();
		bool drive(float timeStep, AnimationBlender* pBlender, const Transform& world);
		bool addToWorld(World* pWorld);
		bool removeFromWorld(World* pWorld);
		void setUserData(void*);

		bool getDisplayGeometry(DisplayGeometries& geom);

		COS_DECLARE_ALLOCATOR();

	private:
		hkaRagdollRigidBodyController* m_pController;
		hkaRagdollInstance* m_pInstance;
		void *m_pUD;

		RagdollControllerHavok(const RagdollControllerHavok& other);
		RagdollControllerHavok& operator=(const RagdollControllerHavok& other);
	};
}

#endif // RAGDOLLCONTROLLERHAVOK_INCLUDED
