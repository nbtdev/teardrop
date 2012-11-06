/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TRANSFORMGIZMO_INCLUDED)
#define TRANSFORMGIZMO_INCLUDED

#if defined(WIN32)
#pragma once
#endif

#include "Resource/ResourceHandle.h"
#include "Gfx/GfxMeshInstance.h"

struct Vert;
namespace Teardrop
{
	class ZoneObject;
	class GfxMaterial;
	class GfxVertexFormat;
	class Ray;

	class TransformGizmo
	{
	public:
		// define the valid operations
		enum
		{
			OP_NONE			= 0x00000000,	// no manipulation
			OP_TRANSLATE_X	= 0x00000001,
			OP_TRANSLATE_Y	= 0x00000002,
			OP_TRANSLATE_Z	= 0x00000004,
			OP_ROTATE_X		= 0x00000008,
			OP_ROTATE_Y		= 0x00000010,
			OP_ROTATE_Z		= 0x00000020,
			OP_SCALE_X		= 0x00000040,
			OP_SCALE_Y		= 0x00000080,
			OP_SCALE_Z		= 0x00000100,

			OP_FORCE_DWORD	= 0xFFFFFFFF
		};

		enum ActiveGizmo
		{
			GIZMO_POSITION=0,
			GIZMO_ROTATION,
			GIZMO_SCALE,
		};

		// default c'tor
		TransformGizmo();
		// provide valid pointer to node to adjust, and its owning scene mgr
		TransformGizmo(ZoneObject* pObj);
		// d'tor
		virtual ~TransformGizmo();

		// initializer; creates gizmo mesh/materials
		bool initialize();
		// cleanup method; call when done with gizmo
		bool destroy();
		//! query whether Mr. Gizmo is initialized or not
		bool isInitialized();

		// set the node to manipulate (pass 0 to clear)
		void setObject(ZoneObject* pObj);
		// show the gizmo
		void show();
		// hide the gizmo
		void hide();
		// update the gizmo
		void update(float deltaSeconds);
		// render the gizmo
		void render();
		// see which handle the user wants
		bool castRay(const Ray& ray, Vector4& intersectPoint);
		// get the current operation(s) highlighted on the gizmo
		int getOperations();
		// get the radius of the current gizmo
		float getRadius() const;
		// freeze the current operation (prevents further raycasts)
		void freeze();
		// un-freeze the current operation (allows further raycasts)
		void unfreeze();
		//! get this gizmo's mesh instance (passIndex determines which instance we want)
		GfxMeshInstance& getMeshInstance(size_t passIndex);
		//! get the current gizmo
		ActiveGizmo getGizmo() const;
		//! set the current gizmo
		void setGizmo(ActiveGizmo gizmo);

		// get the current target node
		ZoneObject* getObject();
		// is the gizmo currently shown?
		bool isVisible();

		TD_DECLARE_ALLOCATOR();
	private:

		void createGizmo();
		bool createRotationGizmo(GfxVertexFormat& fmt);
		bool createPositionGizmo(GfxVertexFormat& fmt);
		bool createScaleGizmo(GfxVertexFormat& fmt);
		void highlightOperations();

		ZoneObject* m_pObj;
		HResource m_gizmo[3]; // PRS
		GfxMeshInstance m_inst[2][3];
		Vert* m_pVertData[3][6];
		GfxMaterial* m_pMtl;
		GfxMaterial* m_pMtlTrans;
		GfxMaterial* m_pMtlSelected;
		GfxMaterial* m_pMtlTransSelected;
		ActiveGizmo m_currentGizmo;
		int m_operations;
		bool m_bVisible;
		bool m_bFreezeOperation;

		// not implemented
		TransformGizmo(const TransformGizmo& );
		TransformGizmo& operator=(TransformGizmo&);
	};
}

#endif // TRANSFORMGIZMO_INCLUDED
