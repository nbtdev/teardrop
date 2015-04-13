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
