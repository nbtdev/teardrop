/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CHARACTERPROXYHAVOK_INCLUDED)
#define CHARACTERPROXYHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Physics/CharacterProxy.h"

class hkpCharacterProxy;
class hkpShapePhantom;

namespace Teardrop
{
	class World;

	class CharacterProxyHavok 
		: public CharacterProxy
	{
		hkpCharacterProxy* m_pProxy;
		hkpShapePhantom* m_pPhantom;
		hkpWorld* m_pWorld;
		size_t m_materialId;
		void* m_pUserData;

	public:
		CharacterProxyHavok();
		~CharacterProxyHavok();

		bool initialize(
			Shape* pShape,
			const Vector4& worldPos,
			float mass,
			float maxSlopeInRadians,
			float friction,
			size_t materialId);
		bool release();
		bool update(float deltaT);

		void getWorldTransform(Transform& xform) const;
		void getLinearVelocity(Vector4& vel) const;
		void getAngularVelocity(Vector4& vel) const;

		void setWorldTransform(const Transform& xform);
		void setLinearVelocity(const Vector4& vel);
		void setAngularVelocity(const Vector4& vel);

		void addToWorld(World* pWorld);
		void removeFromWorld(World* pWorld);
		bool isSupported() const;

		void setUserData(void* pData);
		void* getUserData();

		bool getDisplayGeometry(DisplayGeometries& geom);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // CHARACTERPROXYHAVOK_INCLUDED
