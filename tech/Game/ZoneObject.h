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

#if !defined(TDZONEOBJECT_INCLUDED)
#define TDZONEOBJECT_INCLUDED

#include "Game/ComponentHost.h"
#include "Math/AABB.h"
#include "Math/Transform.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	class Stream;
	struct Environment;
	class Component;
	class Vector4;
    class Zone;

	class ZoneObject 
		: public Reflection::Object
		, public ComponentHost
	{
		friend class Zone;
		friend class ObjectLibrary;

	public:
		TD_CLASS(ZoneObject, Object);
		TD_PROPERTY(Name, "Name of this object", String, "(undefined)", 0);
		TD_COMPLEX_PROPERTY(Transform, "Object's 3D transform in world space", Transform, "[(0,0,0),(1,0,0,0),(1,1,1)]", 0);
		
		ZoneObject();
		~ZoneObject();

		//! Object implementation
		bool initialize();
		bool destroy();
		Reflection::Object* clone() const;

		virtual bool update(float deltaT);

		/** Accessors
		*/
		size_t getObjectId() const { return m_objectId; }
		const AABB& getBoundingBox() const { return m_aabb; }
		float getRadius() const { return m_radius; }
		float getDistSq() const { return m_distSq; }
		bool isDynamic() const { return m_bDynamic; }
		const String& getTemplateName() const { return m_templateName; }

		/** Mutators
		*/
		void setPosition(const Vector4& pos); // world space
		void setOrientation(const Quaternion& q); // world space
		void yaw(float radians); // relative motion
		void pitch(float radians); // relative motion
		void roll(float radians); // relative motion
		void translate(const Vector4& trans); // relative motion
		void setDistanceSqFrom(const Vector4& pt); // sets the m_distSq member based on pt
		void setDynamic(bool isDynamic);
		void setRadius(float radius);
		void setTemplateName(const String& name);

		//! ComponentHost implementation
		const Transform& getTransformWS();
		void setBoundingBox(const AABB& aabb);
		bool isOfType(Reflection::ClassDef*);
		void notifyComponentAdded(Component*);
		void notifyComponentRemoved(Component*);

		// Reflection::Object overrides
		String toString() const;

		// serialization helpers
		virtual bool shouldSerialize() const;

		// called to alert the object that it's been added or removed
		virtual void addedToZone(Zone* pZone);
		virtual void removeFromZone(Zone* pZone);

		TD_DECLARE_ALLOCATOR();

		void notifyPropertyChangedLocal(const Reflection::PropertyDef*);

	protected:
		size_t m_objectId;
		Zone* m_pZone;

		AABB m_aabb;
		Vector4 m_aabbOffset;

		// cache bounding-sphere radius so that we don't need to calc it all the time
		float m_radius;
		float m_distSq; // used only for certain operations, such as sorting

		String m_templateName; // used by object library and other serialization systems

		bool m_bNeedsTransformUpdate;
		bool m_bDynamic;
	};
}

#endif // TDZONEOBJECT_INCLUDED
