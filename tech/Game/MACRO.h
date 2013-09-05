/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MACRO_INCLUDED)
#define MACRO_INCLUDED

#include "Game/Avatar.h"
#include "Game/Kinematic.h"
#include "Memory/Allocators.h"
#include <map>

class TiXmlElement;

namespace Teardrop
{
	class MACRO 
		: public Avatar 
		, public Kinematic<float>
	{
	public:
		TD_CLASS(MACRO, Avatar);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(FreeRotation, "Whether or not this MACRO has torso rotation limits",
			bool, false, 0);
		TD_PROPERTY(LinearSelfBraking, "Whether or not this MACRO stops on its own when you release the throttle",
			bool, true, 0);
		TD_PROPERTY(RadialSelfBraking, "Whether or not this MACRO stops turning on its own when you release the steering control",
			bool, true, 0);
		TD_PROPERTY(RotationalSelfBraking, "Whether or not this MACRO stops twisting on its own when you release the rudder",
			bool, true, 0);
		TD_PROPERTY(MaxForwardLinearAccel, "The MACRO's maximum acceleration (forward), in m/s/s",
			float, 50, 0);
		TD_PROPERTY(MaxReverseLinearAccel, "The MACRO's maximum acceleration (backward), in m/s/s",
			float, 30, 0);
		TD_PROPERTY(MaxRadialAccel, "The MACRO's maximum turning acceleration, in degrees/s",
			float, 45, 0);
		TD_PROPERTY(MaxRotationalAccel, "The MACRO's maximum twisting acceleration, in degrees/s",
			float, 50, 0);
		TD_PROPERTY(MaxForwardSpeed, "The MACRO's maximum forward speed (should match the animations), in km/h",
			float, 50, 0);
		TD_PROPERTY(MaxReverseSpeed, "The MACRO's maximum backward speed (should match the animations), in km/h",
			float, 30, 0);
		TD_PROPERTY(MaxRadialVelocity, "The MACRO's maximum rate of turn, in degrees/s",
			float, 60, 0);
		TD_PROPERTY(MaxRotationalVelocity, "The MACRO's maximum rate of twist, in degrees/s",
			float, 90, 0);
		TD_PROPERTY(MaxRotationalDisplacement, "The MACRO's maximum twist in either direction, relative to dead center, in degrees",
			float, 140, 0);
		TD_PROPERTY(LinearSelfBrake, "How fast the MACRO will slow down when you release the throttle, in m/s/s",
			float, 18, 0);
		TD_PROPERTY(RadialSelfBrake, "How fast the MACRO will straighten out when you release the steering, in degrees/s/s",
			float, 30, 0);
		TD_PROPERTY(RotationalSelfBrake, "How fast the MACRO will stop twisting when you release the rudder, in degrees/s/s",
			float, 30, 0); 
		TD_PROPERTY(MaxPitchAngle, "How far the MACRO will lean forward or backward, in degrees",
			float, 45, 0); 

		MACRO();
		virtual ~MACRO();

		static MACRO* create(TiXmlElement* pElem, const char* pBasePath);

		//! Avatar overrides
		bool _initialize();
		bool _destroy();
		bool _update(float deltaT);
		void _handleCommand(const Command& command);

		TD_DECLARE_ALLOCATOR();

	protected:
		void pitch(float value); // value is in range [-1..1]
		void updateAnimations(float deltaT);

		// very temporary, this should be handled in animation?
		size_t m_spineBoneIndex;
		float m_pitch;
	};
}

#endif // MACRO_INCLUDED
