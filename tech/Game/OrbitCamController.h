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

#if !defined(ORBITCAMCONTROLLER_INCLUDED)
#define ORBITCAMCONTROLLER_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	/*
		Free-camera implementation class 
	*/

	class OrbitCamInputFilter;
	class InputFilter;
	class ZoneObject;

	class OrbitCamController 
		: public CameraController
	{
	public:
		TD_CLASS(OrbitCamController, CameraController);

		OrbitCamController();
		virtual ~OrbitCamController();

		//! manually set the distance from the target
		void setDistanceToTarget(float units);
		//! init values from an existing position (relative to target)
		void reinit(const Vector4& pos);
		//! camera will orbit and look-at this point in space 
		void setTargetPos(const Vector4& targetPos);

		/*
			CameraController implementation
		*/
		bool initialize();
		bool destroy();
		bool update(float deltaT);
		InputFilter* getInputFilter() const;
		void injectMouseMove(int absX, int absY, int relX/* =0 */, int relY/* =0 */);
		void injectMouseWheel(int absZ, int relZ/* =0 */);

		void reset();

		//! on a scale of [0..1]; default is 0.3
		void setZoomStepFactor(float factor);

		TD_DECLARE_ALLOCATOR();

	private:
		void evaluate(float deltaT);

		OrbitCamInputFilter* mInputFilter;
		Vector4 mTargetPos;
		float mDist;
		float mAzimuth;
		float mElevation;
		float mZoomFactor;
	};
}

#endif // ORBITCAMCONTROLLER_INCLUDED
