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

#if !defined(COSGFXCAMERA_INCLUDED)
#define COSGFXCAMERA_INCLUDED

#include "Gfx/include/GfxCommon.h"
#include "Math/include/Vector4.h"
#include "Math/include/Matrix44.h"
#include "Math/include/Quaternion.h"
#include "Math/include/Plane.h"

namespace CoS
{
	struct Environment;
	class Ray;

	class GfxCamera
	{
		Matrix44 m_view;
		Matrix44 m_proj;
		Matrix44 m_viewproj;
		Plane m_frustum[6];
		Vector4 m_lookAt;
		Vector4 m_upVector;
		Vector4 m_position;
		Quaternion m_orientation;
		float m_fovY;
		float m_nearClip;
		float m_farClip;
		float m_aspect;
		bool m_bNeedsUpdate;

		// orthographic projection
		bool m_bOrtho;
		float m_width;
		float m_height;

	public:
		GfxCamera();
		~GfxCamera();

		bool initialize();
		bool destroy();
		bool update();

		/** Accessors
		*/
		const Vector4& getLookAt() const;
		const Vector4& getUpVector() const;
		const Vector4& getPosition() const;
		const Quaternion& getOrientation() const;
		float getFovY() const;
		float getNearClip() const;
		float getFarClip() const;
		float getAspect() const;
		const Plane* getFrustumPlanes() const;
		const Matrix44& getViewMatrix() const;
		const Matrix44& getProjectionMatrix() const;
		const Matrix44& getViewProjMatrix() const;
		bool isOrtho() const;
		//! supply a Ray to receive the results, x and y are normalized
		bool getViewportWorldRay(/*out*/Ray& ray, float x, float y) const;

		/** Mutators
		*/
		void setLookAt(const Vector4& lookAt); // world space
		void setUpVector(const Vector4& upVector); // local space
		void setPosition(const Vector4& pos); // world space
		void setOrientation(const Quaternion& ori);
		void setFovY(float fovY);
		void setNearClip(float nearClip);
		void setFarClip(float farClip);
		void setAspect(float aspect);
		void rotate(float radians, const Vector4& axis);
		void rotate(const Quaternion& q);
		void yaw(float radians);
		void pitch(float radians);
		void roll(float radians);
		void translate(const Vector4& trans);
		void setOrtho(bool bOrtho, float width=0, float height=0);

		DECLARE_GFX_ALIGNED_ALLOCATOR();

	private:
		void updateFrustum();
	};
}

#endif // COSGFXCAMERA_INCLUDED
