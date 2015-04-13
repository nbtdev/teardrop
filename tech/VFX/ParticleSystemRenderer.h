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

#if !defined(PARTICLERENDERER_INCLUDED)
#define PARTICLERENDERER_INCLUDED

#include "Game/Component.h"
#include "VFX/ParticleData.h"
#include "Gfx/IMeshInstanceProvider.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace Teardrop
{
	class ParticleSystemRenderer : public Component
	{
		// a system needs only one mesh instance, as the instance structure 
		// contains all of the instanced-rendering information needed to render
		// all of the system's particles.
//		GfxMeshInstance m_inst;
		struct Quad
		{
			Vector4 pos;
			Vector2 texCoord;
		};
		Quad m_quad[4];
		short m_quadIndices[6];

		// camera parameters
		Vector4 m_camPos;
		Quaternion m_camRot;

		ParticleList m_particles;

	public:
		TD_CLASS(ParticleSystemRenderer, Component);
		TD_PROPERTY(Texture, "Filename of texture to use for particles in this system", String, "", 0);

		ParticleSystemRenderer();
		~ParticleSystemRenderer();

		// Object overrides
		bool initialize();
		bool destroy();

		bool update(float deltaT);

//		const GfxMeshInstance& getMeshInstance() const { return m_inst; }
		ParticleList& getParticleList() { return m_particles; }

		// inform us of the camera's world-space position
		void setCameraParameters(const Vector4& camPos, const Quaternion& camRot) { m_camPos = camPos; m_camRot = camRot; }

		TD_DECLARE_ALLOCATOR();

	protected:

	private:
		void onInstanceCreated();
	};
}

#endif // PARTICLERENDERER_INCLUDED
