/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

/*
	class ObjectManip

	Utility class for manipulating zone objects in the app;
	used for offline world editing as well as online user-
	content editing

*/

#if !defined(OBJECTMANIP_INCLUDED)
#define OBJECTMANIP_INCLUDED

#include "Math/Quaternion.h"
#include "Math/Vector4.h"

namespace Teardrop
{
	class Zone;
	class ZoneObject;
	class TransformGizmo;
	class GfxCamera;
	class GfxMaterial;
	class GfxMeshInstance;
	class GfxRenderTarget;
	class InputFilter;

	class ObjectManip
	{
	public:
		ObjectManip();
		virtual ~ObjectManip();

		enum ManipulationMode
		{
			MANIP_NONE,
			MANIP_TRANSLATE,
			MANIP_ROTATE,
			MANIP_SCALE,
		};

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		bool isEnabled() const;
		const GfxMeshInstance& getRenderable(size_t passIndex) const;
		InputFilter& getInputFilter();
		ZoneObject* getSelectedObject();
		ManipulationMode getMode() const;

		void setZone(Zone* pZone);
		void setCamera(GfxCamera* pCam);
		void setRenderWindow(GfxRenderTarget* pWin);
		void setEnabled(bool enabled);
		void setTarget(ZoneObject* pObj);
		void setMode(ManipulationMode mode);
		void pick(int x, int y);
		void manipulate(int x, int y, int z);
		bool hasActiveOperation(int x, int y);

		struct Listener
		{
			virtual void OnObjectChanged(ZoneObject* pObj) = 0;
		};

		void addListener(Listener* pListener);
		void removeListener(Listener* pListener);

		TD_DECLARE_ALLOCATOR();

	private:
		ZoneObject* castRay(int x, int y);
		void notifySelectionChanged();
		void scanForOperations(int x, int y);
		void manipObject(int rx, int ry);
		void translate(int rx, int ry);
		void rotate(int rx, int ry);
		void scale(int rx, int ry);
		void getManipFactors(
			int dx,		// in: screen-space x delta
			int dy,		// in: screen-space y delta
			float& x,	// out: object-local x-axis change factor
			float& y,	// out: object-local y-axis change factor
			float& z);	// out: object-local z-axis change factor

		struct _Listeners;
		_Listeners* m_pListeners;

		Zone* m_pZone;
		ZoneObject* m_pSelected;
		GfxCamera* m_pCamera;
		GfxRenderTarget* m_pWindow;
		TransformGizmo* m_pGizmo;
		ManipulationMode m_mode;
		Quaternion m_rotateStart;
		Vector4 m_transScaleStart;
		Vector4 m_transOriginal;
		float m_rotateStartAngle;
		bool m_bLMB;
		bool m_bEnabled;
	};
}

#endif // OBJECTMANIP_INCLUDED
