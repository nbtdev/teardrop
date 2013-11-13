/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTVIEWER3D_INCLUDED)
#define OBJECTVIEWER3D_INCLUDED

#include <QWidget>

class QTimer;

namespace Teardrop
{
	class Package;
	class OrbitCameraController;

	namespace Gfx {
		class Renderer;
		class RenderTarget;
		class Viewport;
	}

	namespace Tools
	{
		class PropertyGrid;

		class ObjectViewer3D : public QWidget
		{
			Q_OBJECT 

		public:
			ObjectViewer3D(Gfx::Renderer* renderer, QWidget* parent=0);
			~ObjectViewer3D();

			Package* package();

		protected slots:
			void onIdle();

		protected:
			Gfx::Renderer* mRenderer;
			Gfx::RenderTarget* mRenderWindow;
			Gfx::Viewport* mViewport;
			Package* mPackage;
			OrbitCameraController* mOrbitCam;
			
			QWidget* m3DView;
			PropertyGrid* mPropGrid;
			QTimer* mTimer;
		};
	}
}

#endif // OBJECTVIEWER3D_INCLUDED
