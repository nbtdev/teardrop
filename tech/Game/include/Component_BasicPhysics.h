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

#if !defined(COMPONENT_BASICPHYSICS_INCLUDED)
#define COMPONENT_BASICPHYSICS_INCLUDED

#include "Game/include/Component_Physics.h"
#include "Resource/include/ResourceHandle.h"
#include "Memory/include/Allocators.h"

namespace CoS
{
	class Body;
	class World;

	class BasicPhysicsComponent 
		: public PhysicsComponent
	{
		HResource m_hBody;
		World* m_pWorld;

		enum Operation
		{
			NONE = 0,
			ADD,
			REMOVE,
		};
		Operation m_pendingOp;

	public:
		COS_CLASS(BasicPhysicsComponent, PhysicsComponent);
		COS_CLASS_CREATABLE();

		COS_PROPERTY(RigidBodyName, "Name of rigid body asset file to use", String, "(undefined)", FileChooser);

		BasicPhysicsComponent();
		virtual ~BasicPhysicsComponent();

		bool initialize();
		bool destroy();
		bool update(float deltaT);
		bool addToWorld(World* pWorld);
		bool removeFromWorld(World* pWorld);
		bool getWorldTransform(Transform& xform);
		bool setWorldTransform(const Transform& xform);
		bool setBody(const char* assetFilename);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		COS_DECLARE_ALLOCATOR();

	protected:
		void initializeDebugMesh();
	};
}

#endif // COMPONENT_BASICPHYSICS_INCLUDED
