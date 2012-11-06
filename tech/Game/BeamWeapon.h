/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BEAMWEAPON_INCLUDED)
#define BEAMWEAPON_INCLUDED

#include "Game/Weapon.h"
#include "Gfx/GfxMeshInstance.h"

namespace Teardrop
{
	class GfxShaderConstant;
	class CountdownTrigger;

	class BeamWeapon
		: public Weapon
	{
		GfxMeshInstance m_inst;
		GfxShaderConstant* m_pMountTransformConstant;

	public:
		TD_CLASS(BeamWeapon, Weapon);

		BeamWeapon();
		~BeamWeapon();

		// Component overrides
		bool _initialize();
		bool _destroy();
		bool _update(float deltaT);

		// Weapon implementation
		void fire();

		// Trigger callback(s)
		void playRecycleSfx();

		TD_DECLARE_ALLOCATOR();

	private:

		struct Vertex
		{
			float px, py, pz;
			float tx, ty;
			TD_DECLARE_ALLOCATOR();
		};

		Vertex* m_pVertexData;

		// firing control
		float m_animationTimeRemaining;
		float m_recycleTimeRemaining;
	};
}

#endif // BEAMWEAPON_INCLUDED
