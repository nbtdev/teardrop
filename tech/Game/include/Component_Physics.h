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

#if !defined(COMPONENT_PHYSICS_INCLUDED)
#define COMPONENT_PHYSICS_INCLUDED

#include "Game/include/Component.h"
#include "Gfx/include/GfxMeshInstance.h"

namespace CoS
{
	class ZoneObject;
	class World;
	class Transform;
	struct FourCC;

	class PhysicsComponent
		: public Component
	{
	public:
		COS_CLASS(PhysicsComponent, Component);

		PhysicsComponent();
		~PhysicsComponent();

		virtual bool initialize(const char* asset, const FourCC& type);
		virtual bool destroy();
		virtual bool update(float deltaT);
		virtual bool addToWorld(World* pWorld);
		virtual bool removeFromWorld(World* pWorld);
		virtual bool getWorldTransform(Transform& xform);
		virtual bool setWorldTransform(const Transform& xform);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		// for debug display
		virtual GfxMeshInstance& getDebugMesh();

	protected:
		GfxMeshInstance m_debugMeshInstance;
		bool m_bDebugMeshInitialized;

		virtual void initializeDebugMesh();
		virtual void releaseDebugMesh();
	};
}

#endif // COMPONENT_PHYSICS_INCLUDED
