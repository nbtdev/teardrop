/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BODYHAVOK_INCLUDED)
#define BODYHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Physics/Body.h"

class hkpRigidBody;
class hkResource;

namespace Teardrop
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

		TD_DECLARE_ALLOCATOR();

	protected:
		SerialPointer<hkResource> m_pResource;
		SerialPointer<hkpRigidBody> m_pBody;

	private:
		void delayInit();
	};
}

#endif // BODYHAVOK_INCLUDED
