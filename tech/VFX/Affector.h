/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AFFECTOR_INCLUDED)
#define AFFECTOR_INCLUDED

#include "VFX/ParticleData.h"
#include "Game/Component.h"
#include "Math/Vector4.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class Affector : public Component
	{
	public:
		TD_CLASS(Affector, Component);

		Affector();
		~Affector();

		// Object overrides
		bool initialize();
		bool destroy();

		virtual bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	protected:

	private:
		void onInstanceCreated();
	};
}

#endif // AFFECTOR_INCLUDED
