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
