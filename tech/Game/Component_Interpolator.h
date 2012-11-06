/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_INTERPOLATOR_INCLUDED)
#define COMPONENT_INTERPOLATOR_INCLUDED

#include "Memory/Allocators.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Game/Component.h"

namespace Teardrop
{
	class ZoneObject;

	class InterpolatorComponent 
		: public Component
	{
		ZoneObject* m_pHost;
		Vector4 m_syncPos; // cache the latest sync position (i.e. for networked MP interpolation)
		Quaternion m_syncRot; // cache latest orientation sync
		float m_syncInterval; // track the time between syncs, for interpolation
		float m_accumTime;	  // how much time has passed this interval
		float m_timeStep;

	public:
		TD_CLASS(InterpolatorComponent, Component);
		TD_CLASS_CREATABLE();

		InterpolatorComponent();
		virtual ~InterpolatorComponent();

		bool update(float deltaT);
		void setSync(const Vector4& pos, const Quaternion& rot); // used to set an interpolation vector (for networked entities)
		void getVelocityAdjustment(
			/*in*/ const Vector4& currentPos, 
			/*out*/ Vector4& adjust);
		void getRotationAdjustment(
			/*in*/ const Quaternion& currentRot, 
			/*out*/ Quaternion& adjust);

		TD_DECLARE_ALLOCATOR();

	protected:

	private:
		void onInstanceCreated();
	};
}

#endif // COMPONENT_INTERPOLATOR_INCLUDED
