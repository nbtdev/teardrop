/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWidget>

class QTimer;

namespace Teardrop
{
	class GfxRenderer;
	class GfxCamera;
	class GfxViewport;
	class GfxRenderTarget;
	class Scene;
	class SceneRenderer;

	namespace Tools {
		class RenderWindow : public QWidget
		{
			Q_OBJECT

		public:
			RenderWindow(QWidget* parent=0);
			~RenderWindow();

			void setScene(Scene* scene);

		protected slots:
			void onIdle();

		private:
			QTimer* mTimer;
			GfxRenderer* mRenderer;
			GfxCamera* mCamera;
			GfxViewport* mViewport;
			GfxRenderTarget* mRT;
			Scene* mScene;
			SceneRenderer* mSceneRenderer;
		};
	}
}

#endif // RENDERWINDOW_H
