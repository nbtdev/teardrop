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

#if !defined(COMPONENT_INTERPOLATOR_INCLUDED)
#define COMPONENT_INTERPOLATOR_INCLUDED

#include "Memory/Allocators.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Game/Component.h"

namespace CoS
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
		COS_CLASS(InterpolatorComponent, Component);
		COS_CLASS_CREATABLE();

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

		COS_DECLARE_ALLOCATOR();

	protected:

	private:
		void onInstanceCreated();
	};
}

#endif // COMPONENT_INTERPOLATOR_INCLUDED
