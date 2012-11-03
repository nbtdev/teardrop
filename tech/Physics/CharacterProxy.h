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

#if !defined(CHARACTERPROXY_INCLUDED)
#define CHARACTERPROXY_INCLUDED

#include <vector>
#include <list>

namespace CoS
{
	class Shape;
	class World;
	class Vector4;
	struct FourCC;
	class Transform;

	class CharacterProxy
	{
	public:
		CharacterProxy();
		virtual ~CharacterProxy();

		virtual bool initialize(
			Shape* pShape,
			const Vector4& worldPos,
			float mass,
			float maxSlopeInRadians,
			float friction,
			size_t materialId
			) = 0;

		virtual bool release() = 0;
		virtual bool update(float deltaT) = 0;

		virtual void getWorldTransform(Transform& xform) const = 0;
		virtual void getLinearVelocity(Vector4& vel) const = 0;
		virtual void getAngularVelocity(Vector4& vel) const = 0;

		virtual void setWorldTransform(const Transform& xform) = 0;
		virtual void setLinearVelocity(const Vector4& vel) = 0;
		virtual void setAngularVelocity(const Vector4& vel) = 0;

		virtual void addToWorld(World* pWorld) = 0;
		virtual void removeFromWorld(World* pWorld) = 0;

		virtual void setUserData(void*) = 0;
		virtual void* getUserData() = 0;

		virtual bool isSupported() const = 0;

		// debug view of the proxy's shape
		typedef std::vector<Vector4> DisplayGeometry;
		typedef std::list<DisplayGeometry> DisplayGeometries;
		// will return a list of lines (start end start end ...) that describe 
		// the debug view of the proxy's geometry
		virtual bool getDisplayGeometry(DisplayGeometries& geom) = 0;
	};
}

#endif // CHARACTERPROXY_INCLUDED
