/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PARTICLESYSTEM_INCLUDED)
#define PARTICLESYSTEM_INCLUDED

#include "VFX/ParticleData.h"
#include "Game/ZoneObject.h"
#include <vector>

namespace Teardrop
{
	class GfxMesh;
	class GfxVertexFormat;
	class GfxVertexData;
	class ScriptVM;
	class Emitter;
	class Affector;

	class ParticleSystem : public ZoneObject
	{
		// all of the particles in this system
		ParticleDataPool m_particles;

		// script VM instance, if Script property is set
		ScriptVM* m_pScript;

	public:
		TD_CLASS(ParticleSystem, ZoneObject);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Script, "Name of script that defines this particle system", String, "(undefined)", 0);
		TD_PROPERTY(SystemQuota, "Max number of particles supported by this system instance", int, 100, 0);

		ParticleSystem();
		~ParticleSystem();

		// Object overrides
		bool initialize();
		bool destroy();
		// ZoneObject overrides
		bool update(float deltaT);

		// inform us of the camera's world-space position
		void setCameraParameters(const Vector4& camPos, const Quaternion& camRot);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // PARTICLESYSTEM_INCLUDED
