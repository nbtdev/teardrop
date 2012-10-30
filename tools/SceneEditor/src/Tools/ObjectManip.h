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

/*
	class ObjectManip

	Utility class for manipulating zone objects in the app;
	used for offline world editing as well as online user-
	content editing

*/

#if !defined(OBJECTMANIP_INCLUDED)
#define OBJECTMANIP_INCLUDED

#include "Math/include/Quaternion.h"
#include "Math/include/Vector4.h"

namespace CoS
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

		COS_DECLARE_ALLOCATOR();

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
