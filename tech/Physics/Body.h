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

#if !defined(BODY_INCLUDED)
#define BODY_INCLUDED

#include "Math/Vector4.h"
#include "Memory/Allocators.h"
#include <vector>
#include <list>

namespace Teardrop
{
	class Shape;
	class Vector4;
	class Transform;

	class Body 
	{
	public:
		enum MotionType
		{
			STATIC,
			DYNAMIC,
			KEYFRAMED
		};

		//! normal c'tor (cannot fail)
		Body();
		//! placement c'tor (cannot fail)
		Body(int);
		//! d'tor (cannot fail)
		virtual ~Body();

		virtual bool initialize(
			void* pData,
			unsigned int dataLen
			);

		virtual bool initialize(
			Shape* pShape,
			MotionType motionType,
			const Vector4& origin = Vector4::ZERO,
			const Vector4& inertia = Vector4::UNIT_SCALE
			);

		virtual bool update(float deltaT);

		Shape* getShape();
		virtual void changeMotionType(MotionType type);

		virtual void getLinearVelocity(Vector4& vel);
		virtual void getAngularVelocity(Vector4& vel);
		virtual void getWorldTransform(Transform& xform);
		virtual void* getUserData();

		virtual void setLinearVelocity(const Vector4& vel);
		virtual void setAngularVelocity(const Vector4& vel);
		//! this one is sort of dangerous -- use only for initial world setup
		virtual void setWorldTransform(const Transform& xform);
		virtual void setUserData(void* pData);

		bool destroy();

		// make a copy of this object with unique rigid body
		virtual Body* clone();

		// debug view of the body's shape
		typedef std::vector<Vector4> DisplayGeometry;
		typedef std::list<DisplayGeometry> DisplayGeometries;
		// will return a list of lines (start end start end ...) that describe 
		// the debug view of the body's geometry
        //virtual bool getDisplayGeometry(DisplayGeometries& geom);

		TD_DECLARE_ALLOCATOR();

	protected:
        void* m_pData;
        void* m_pUserData;
        Shape* m_pShape;
		unsigned int m_dataLen;
	};
}

#endif // BODY_INCLUDED
