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

#if !defined(CHARACTERPROXYHAVOK_INCLUDED)
#define CHARACTERPROXYHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "CharacterProxy.h"

class hkpCharacterProxy;
class hkpShapePhantom;

namespace CoS
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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // CHARACTERPROXYHAVOK_INCLUDED
