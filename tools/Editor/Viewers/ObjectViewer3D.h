/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTVIEWER3D_INCLUDED)
#define OBJECTVIEWER3D_INCLUDED

#include <QtWidgets/QWidget>
#include <memory>

class QTimer;

namespace Teardrop
{
	class Package;
	class OrbitCameraController;
    class String;

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
            void setObjectName(const String& aName);

        public slots:
            void onClose();

		protected slots:
			void onIdle();
			void mouseMoveEvent(QMouseEvent* event);

		protected:
			void resizeEvent(QResizeEvent *event);

			Gfx::Renderer* mRenderer;
			std::shared_ptr<Gfx::RenderTarget> mRenderWindow;
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
