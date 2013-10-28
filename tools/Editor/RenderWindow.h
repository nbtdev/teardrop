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
	namespace Gfx
	{
		class Renderer;
		class RenderTarget;
	}

	namespace Tools {
		class RenderWindow : public QWidget
		{
			Q_OBJECT

		public:
			RenderWindow(QWidget* parent=0);
			~RenderWindow();

		protected slots:
			void onIdle();

		private:
			QTimer* mTimer;
			Gfx::Renderer* mRenderer;
			Gfx::RenderTarget* mRT;
		};
	}
}

#endif // RENDERWINDOW_H
