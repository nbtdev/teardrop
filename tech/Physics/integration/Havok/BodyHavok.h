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

#if !defined(BODYHAVOK_INCLUDED)
#define BODYHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Physics/Body.h"

class hkpRigidBody;
class hkResource;

namespace CoS
{
	class BodyHavok : public Body
	{
		DECLARE_SERIALIZABLE(BodyHavok);

	public:
		//! normal c'tor (cannot fail)
		BodyHavok();
		//! placement c'tor (cannot fail)
		BodyHavok(int);
		//! d'tor (cannot fail)
		~BodyHavok();

		bool initialize(
			void* pData,
			unsigned int dataLen
			);

		bool initialize(
			Shape* pShape,
			MotionType motionType,
			const Vector4& origin = Vector4::ZERO,
			const Vector4& inertia = Vector4::UNIT_SCALE
			);

		bool release();
		bool update(float deltaT);
		void changeMotionType(MotionType type);

		hkpRigidBody* getHavokBody();

		void getLinearVelocity(Vector4& vel);
		void getAngularVelocity(Vector4& vel);
		void getWorldTransform(Transform& xform);
		void* getUserData();

		void setLinearVelocity(const Vector4& vel);
		void setAngularVelocity(const Vector4& vel);
		void setWorldTransform(const Transform& xform);
		void setUserData(void* pData);

		Body* clone();

		bool getDisplayGeometry(DisplayGeometries& geom);

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

		COS_DECLARE_ALLOCATOR();

	protected:
		SerialPointer<hkResource> m_pResource;
		SerialPointer<hkpRigidBody> m_pBody;

	private:
		void delayInit();
	};
}

#endif // BODYHAVOK_INCLUDED
