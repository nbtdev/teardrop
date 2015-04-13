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

#if !defined(__KINEMATIC_H__)
#define __KINEMATIC_H__

#include "Math/Vector4.h"
#include "Math/MathUtil.h"
#include <algorithm>

namespace Teardrop {
	/*

	Kinematic is a simple generalized kinematic point-mass control system, that follows 
	basic dynamic laws of motion. The two linear quantities computed are position and velocity,
	and those two change based on the amount of acceleration applied (and direction, forward or
	reverse, as flagged by the sign of the acceleration). Linear acceleration is input #1. 
	One radial quantity is also computed: angular velocty, based on	the radial acceleration applied 
	(input #2). The "rest" position assumes no particular Quaternion; it is up to the user to 
	interpret the current position. By convention the rest position is "forward". A third, independent 
	set of radial values is computed for independent "twist", i.e. for turrets on tanks or torsos on 
	robotic entities. Input #3 is the radial acceleration of this twist factor, and radial twist 
	displacement as well as radial velocity are computed for this twist factor.

	The inputs are all based on a throttle/steering/rudder system, where throttle controls forward/backward
	motion, steering controls left/right motion, and rudder controls any rotational (twisting) motion. These
	inputs occupy the space between -1.0 and 1.0, and the actual accelerations to which they correspond are
	scaled according to the maximal acceleration values registered with the controller. 

	The controller also supports "releasing" of throttle and rudder controls by consideration of a maximum
	amount of change between evaluations, beyond which the controller will consider the control "released" and
	apply a braking force to the relevant motion (this prevents instant stopping and allows for a more 
	realistic experience without having to code for it). This concept is also known as the "deadman's brake".

	Full controllable braking is not yet implemented, even though the placeholders are available.

	Radial and rotational motion are considered symmetric (their maxima are the same in both directions) but
	the linear motion is considered asymmetric (maximal forward and reverse accelerations and maxima are not
	assumed to be the same; they can be, but they are not assumed to be). 

	Each position and displacement is evaluated every time evaluate() is called. evaluate() takes a single
	paramater of time since last update, and recomputes all displacements based on the inputs supplied.

	Limiting factors such as maximum speed, maximum radial velocity of both turn and twist, and maximum
	twist displacement, are supplied as initialization parameters but can be set at will at any time. These
	factors have no default values; they are all set to zero at construction of this class.

	This class is templated with the type of precision desired. Typically this means "float" or "double" in 
	IEEE terms; the mathematics make no assumption about the precision or type of the numbers involved.
		
	The displacment amounts used follow a right-handed coordinate system for 3D positions: +X is to the right,
	+Y is up, and +Z is forward. For rotational and radial values, + is to the left and - is to the right.

	Radial and rotational displacements are in radians, which are on the basis of "pi" (3.1415927 roughly).
	2*pi = 360 degrees (6.2831853), so pi = 180 degrees and pi/2 (1.5707963) = 90 degrees.
	*/

	template <class P> class Kinematic {
	public:
		Kinematic() {
			m_throttle = (P)0.0;					// throttle factor
			m_steer = (P)0.0;						// steering (turning) factor
			m_rudder = (P)0.0;						// rudder (twisting) factor
			m_linearVelocity = (P)0.0;				// current linear (forward/backward) velocity
			m_radialVelocity = (P)0.0;				// current radial (turning, left/right) velocity
			m_rotationalVelocity = (P)0.0;			// current rotational (twisting) velocity

			// displacement values 
			m_rotationalDisplacement = (P)0.0;		// amount of rotation (twist) caused by previous evaluation
			m_radialDisplacement = (P)0.0;			// amount of turn caused by previous evaluation

			// limiting values
			m_maxForwardLinearAccel = (P)0.0;		// maximum acceleration allowed (forward)
			m_maxReverseLinearAccel = (P)0.0;		// maximum acceleration allowed (reverse)
			m_maxForwardSpeed = (P)0.0;				// maximum forward speed 
			m_maxReverseSpeed = (P)0.0;				// maximum reverse speed 
			m_maxRadialVelocity = (P)0.0;			// maximum turning speed
			m_maxRotationalVelocity = (P)0.0;		// maximum twisting speed
			m_maxRadialAccel = (P)0.0;				// maximum radial acceleration
			m_maxRotationalAccel = (P)0.0;			// maximum rotational acceleration
			m_maxRotationalDisplacement = (P)0.0;	// maximum rotational displacement, in radians (wraps at pi)
			m_freeRotation = false;					// if true, max rotational displacement is ignored; displacement values still wrap at pi

			// damping values 
			m_maximumThrottleDelta = (P)0.0;		// amount of change since last evaluation to consider throttle "released"
			m_maximumRudderDelta = (P)0.0;			// amount of change since last evaluation to consider rudder "released"
			m_linearSelfBrake = (P)0.0;				// "deceleration" characteristic when throttle is released
			m_radialSelfBrake = (P)0.0;				// "deceleration" characteristic when throttle is released
			m_rotationalSelfBrake = (P)0.0;			// twist deceleration characteristic when rudder is released


			// braking values
			m_maximumLinearBrake = (P)0.0;			// not yet implemented

			m_linearSelfBraking = false;
			m_radialSelfBraking = false;
			m_rotationalSelfBraking = false;
			m_braking = false;

			ERRORVALUE = (P)0.05;

			m_positionalDisplacement = Vector4(0,0,0,0);
		}

		~Kinematic() {
		}


		// provide current throttle factor -- clamps to 1.0 magnitude
		void throttle(const P throttle) {
			m_throttle = throttle;

			if (throttle > (P)1.0)
				m_throttle = (P)1.0;
			if (throttle < (P)(-1.0))
				m_throttle = (P)(-1.0);

			// if throttle goes to zero then do self-braking; otherwise apply maximum acceleration
			if (MathUtil::abs(throttle) <= ERRORVALUE) {
				m_linearSelfBraking = true;
				m_throttle = (P)0.0;
			}
			else {
				m_linearSelfBraking = false;
			}
		}

		// get current throttle factor
		inline const P throttle() { return m_throttle; }

		// provide current steering factor (turn) -- clamps to 1.0 magnitude
		void steer(const P steer) {
			m_steer = steer;

			if (steer > (P)1.0)
				m_steer = (P)1.0;
			if (steer < (P)(-1.0))
				m_steer = (P)(-1.0);

			// if steering goes to zero then do self-braking; otherwise apply maximum acceleration
			if (MathUtil::abs(steer) <= ERRORVALUE) {
				m_radialSelfBraking = true;
				m_steer = (P)0.0;
			}
			else {
				m_radialSelfBraking = false;
			}
		}

		// get current steering factor (turn)
		inline const P steer() { return m_steer; }

		// provide current rudder factor (twist) -- clamps to 1.0 magnitude
		void rudder(const P rudder) { 
			m_rudder = rudder;

			if (rudder > (P)1.0)
				m_rudder = (P)1.0;
			if (rudder < (P)(-1.0))
				m_rudder = (P)(-1.0);

			// if throttle goes to zero then do self-braking; otherwise apply maximum acceleration
			if (MathUtil::abs(rudder) < ERRORVALUE) {
				m_rotationalSelfBraking = true;
				m_rudder = (P)0.0;
			}
			else
				m_rotationalSelfBraking = false;
		}

		// get current rudder factor (twist)
		inline const P rudder() { return m_rudder; }

		// get the current positional displacement resulting from previous update -- this factors in linear and radial properties
		inline const Vector4 displacement() { return m_positionalDisplacement; }

		// get the current rotational displacement resulting from previous update -- this account for rotational (twist) properties only
		inline const P rotation() { return m_rotationalDisplacement; }

		// apply braking force (not yet implemented)
		inline void brake(P brakeAmount) {}

	protected:
		// dynamic values 
		P m_throttle;					// throttle factor
		P m_steer;						// steering (turning) factor
		P m_rudder;						// rudder (twisting) factor
		P m_linearVelocity;				// current linear (forward/backward) velocity
		P m_radialVelocity;				// current radial (turning, left/right) velocity
		P m_rotationalVelocity;			// current rotational (twisting) velocity

		// displacement values 
		Vector4 m_positionalDisplacement; // amount of displacement caused by previous evaluation THIS STEP ONLY
		P m_rotationalDisplacement;		// amount of rotation (twist) caused by previous evaluation THIS STEP ONLY
		P m_radialDisplacement;			// amount of angular displacement THIS STEP ONLY

		// limiting values
		P m_maxForwardLinearAccel;		// maximum acceleration allowed (forward)
		P m_maxReverseLinearAccel;		// maximum acceleration allowed (reverse)
		P m_maxForwardSpeed;			// maximum forward speed 
		P m_maxReverseSpeed;			// maximum reverse speed 
		P m_maxRadialVelocity;			// maximum turning speed
		P m_maxRotationalVelocity;		// maximum twisting speed
		P m_maxRadialAccel;				// maximum radial acceleration
		P m_maxRotationalAccel;			// maximum rotational acceleration
		P m_maxRotationalDisplacement;	// maximum rotational acceleration
		bool m_freeRotation;			// is true, max rotation is ignored (current displacement still wraps at pi)

		// damping values 
		P m_maximumThrottleDelta;		// amount of change since last evaluation to consider throttle "released"
		P m_maximumRudderDelta;			// amount of change since last evaluation to consider rudder "released"
		P m_linearSelfBrake;			// "deceleration" characteristic when throttle is released
		P m_radialSelfBrake;			// "deceleration" characteristic when throttle is released
		P m_rotationalSelfBrake;		// twist deceleration characteristic when rudder is released


		// braking values
		P m_maximumLinearBrake;			// not yet implemented

		// braking flags
		bool m_linearSelfBraking;
		bool m_radialSelfBraking;
		bool m_rotationalSelfBraking;
		bool m_braking;

		// manifest values
		P ERRORVALUE;

	public:
		// update values over supplied time slice (time in milliseconds for enhanced precision)
		void evaluate(const P ms) {

			P tDelta = ms;// / (P)1000.0;

			/*
				Assumptions we can make in the processing:
				* since we are only providing deltas, then initial position p0 can always be zero
				* "initial" velocities are the velocities at the end of the previous evaluation
			*/

			// figure out actual linear acceleration from forward/reverse throttle and braking values
			P linearAccel = (P)0.0;
			P targetVelocity = (P)0.0;

			// if the throttle is < 0.0, then the target velocity is negative too. If we are self-braking
			// then apply the self-braking term, otherwise apply the maximal reverse acceleration
			if (m_linearSelfBraking) {
				if (m_linearVelocity < (P)0.0)
					linearAccel = m_linearSelfBrake;
				else
					linearAccel = -m_linearSelfBrake;
			}
			else {
				if (m_throttle > (P)0.0) {
					linearAccel = m_maxForwardLinearAccel;
					targetVelocity = m_throttle * m_maxForwardSpeed;
				}
				else {
					linearAccel = -m_maxReverseLinearAccel;
					targetVelocity = m_throttle * (-m_maxReverseSpeed);
				}
			}

			// adjust current velocity to reflect new acceleration (if any).
			// If velocity:
			//  (1) is near the throttle value,
			//  (2) is clamped to a maximum,
			//	(3) is near zero with the throttle released
			// then do not change velocity
			if (m_throttle == (P)0.0 && MathUtil::abs(m_linearVelocity - targetVelocity) <= ERRORVALUE || 
				m_throttle != (P)0.0 && (m_linearVelocity < m_maxReverseSpeed || m_linearVelocity > m_maxForwardSpeed))
			{
				linearAccel = (P)0.0;
			}

			// v = v0 + at
			// p = p0 + v0t + (1/2)at^2
			P zDelta;
			if (MathUtil::abs(linearAccel) > ERRORVALUE) {
				P newVelocity = m_linearVelocity + linearAccel * tDelta;
				if (m_throttle == (P)0.0 && (newVelocity * m_linearVelocity <= (P)0.0))
					m_linearVelocity = (P)0.0;
				else
				{
					if (newVelocity < P(0))
					{
						newVelocity = std::max(newVelocity, m_maxReverseSpeed);
					}
					else
					{
						newVelocity = std::min(newVelocity, m_maxForwardSpeed);
					}

					m_linearVelocity = newVelocity;
				}

				zDelta = m_linearVelocity * tDelta + (P)0.5 * linearAccel * tDelta * tDelta;
			} 
			else {
				// skip the voided multiplications
				zDelta = m_linearVelocity * tDelta;
			}

			// Z change is the forward/backward component of the position; linear acceleration is 
			//		already taken into account by the general linear formula above
			// X change is the side-to-side motion and takes into consideration the Z component and the 
			//		radial velocity (after applying the radial acceleration for this evaluation)
			// The change in X is a MathUtil::sin() function, multiplied by the Z change. 
			// The actual radial change depends on the current radial velocity in rad/s; we can tell how
			// many radians we've been through by multiplying by tDelta (which is in ms) and dividing by
			// 1000 (to get units of seconds). This is the angular value supplied to the MathUtil::sin() function, 
			// and after that it's basic high-school trig. The radial velocity is clamped similar to above
			// and like above, calculated only if it needs to be. 

			// if the throttle is < 0.0, then the target velocity is negative too. If we are self-braking
			// then apply the self-braking term, otherwise apply the maximal reverse acceleration
			P targetRadialVelocity = m_steer * m_maxRadialVelocity;
			P radialAccel = (P)0.0;
			if (m_radialSelfBraking) {
				if (m_radialVelocity < (P)0.0)
					radialAccel = m_radialSelfBrake;
				else
					radialAccel = -m_radialSelfBrake;
			}
			else {
				if (m_steer > (P)0.0) {
					radialAccel = m_maxRadialAccel;
				}
				else {
					radialAccel = -m_maxRadialAccel;
				}
			}

			// adjust current velocity to reflect new acceleration (if any).
			// If velocity:
			//  (1) is near the steering value,
			//  (2) is clamped to a maximum
			// then do not change velocity
			if (MathUtil::abs(m_radialVelocity - targetRadialVelocity) <= ERRORVALUE || 
				m_steer != (P)0.0 && (m_radialVelocity < -m_maxRadialVelocity || m_radialVelocity > m_maxRadialVelocity))
				radialAccel = (P)0.0;

			// v = v0 + at
			P xDelta;
			if (MathUtil::abs(radialAccel) > ERRORVALUE) {
				P newVelocity = m_radialVelocity + radialAccel * tDelta;

				if (newVelocity < P(0))
				{
					newVelocity = std::max(newVelocity, -m_maxRadialVelocity);
				}
				else
				{
					newVelocity = std::min(newVelocity, m_maxRadialVelocity);
				}

				if ((MathUtil::abs(m_steer) < ERRORVALUE) && (MathUtil::abs(newVelocity) <= ERRORVALUE || (newVelocity * m_radialVelocity <= (P)0.0)))
					m_radialVelocity = (P)0.0;
				else
					m_radialVelocity = newVelocity;
			} 

			m_radialDisplacement = m_radialVelocity * tDelta;
			if (m_steer > (P)0.0)
				xDelta = -zDelta * MathUtil::sin (m_radialDisplacement);
			else if (m_steer < (P)0.0)
				xDelta = zDelta * MathUtil::sin (m_radialDisplacement);
			else
				xDelta = (P)0.0;

			m_positionalDisplacement.x = xDelta;
			m_positionalDisplacement.y = (P)0.0;
			m_positionalDisplacement.z = zDelta * MathUtil::cos (m_radialDisplacement);


			// if the rudder is < 0.0, then the target velocity is negative too. If we are self-braking
			// then apply the self-braking term, otherwise apply the maximal reverse acceleration
			P targetRotationalVelocity = m_rudder * m_maxRotationalVelocity;
			P rotationalAccel = (P)0.0;
			if (m_rotationalSelfBraking) {
				if (m_rotationalVelocity < (P)0.0)
					rotationalAccel = m_rotationalSelfBrake;
				else
					rotationalAccel = -m_rotationalSelfBrake;
			}
			else {
				if (m_rudder > (P)0.0) {
					rotationalAccel = m_maxRotationalAccel;
				}
				else {
					rotationalAccel = -m_maxRotationalAccel;
				}
			}

			// if the velocity is near where we want it, stop accelerating
			if (MathUtil::abs(m_rotationalVelocity - targetRotationalVelocity) <= ERRORVALUE)
				rotationalAccel = (P)0.0;

			// v = v0 + at
			P newVelocity;
			if (rotationalAccel != (P)0.0)
				newVelocity = m_rotationalVelocity + rotationalAccel * tDelta;
			else
				newVelocity = m_rotationalVelocity;

			if (MathUtil::abs(m_rudder) < ERRORVALUE) {
				// if the rudder is at near zero, and so is the rotation, then stop the rotation at zero
				if (MathUtil::abs(newVelocity) <= ERRORVALUE || (newVelocity * m_rotationalVelocity <= (P)0.0)) {
					newVelocity = (P)0.0; 
					//m_rotationalDisplacement = (P)0.0;
				}
			}

			P rDelta = newVelocity * tDelta;
			P newDisplacement = m_rotationalDisplacement + rDelta;

			//char buf[255];
			//sprintf (buf, "rdisp, rdelta, rVel, newVel, newDisp, rudder: %f, %f, %f, %f, %f, %f\n",
			//	m_rotationalDisplacement, tDelta, m_rotationalVelocity, newVelocity, newDisplacement, m_rudder);
			//OutputDebugString(buf);

			if (!m_freeRotation && (newDisplacement > m_maxRotationalDisplacement || newDisplacement < (-m_maxRotationalDisplacement))) {
				m_rotationalVelocity = (P)0.0;;
			}
			else {
				m_rotationalVelocity = newVelocity;
				m_rotationalDisplacement += rDelta;
			}

		}

	};
} // namespace MathUtil::cos

#endif // __KINEMATIC_H__
