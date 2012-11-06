/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PARTICLERENDERER_INCLUDED)
#define PARTICLERENDERER_INCLUDED

#include "Game/Component.h"
#include "VFX/ParticleData.h"
#include "Gfx/GfxMeshInstance.h"
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
		GfxMeshInstance m_inst;
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

		const GfxMeshInstance& getMeshInstance() const { return m_inst; }
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
