/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXCAMERA_INCLUDED)
#define TDGFXCAMERA_INCLUDED

#include "Gfx/GfxCommon.h"
#include "Math/Vector4.h"
#include "Math/Matrix44.h"
#include "Math/Quaternion.h"
#include "Math/Plane.h"

namespace Teardrop
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

#endif // TDGFXCAMERA_INCLUDED
