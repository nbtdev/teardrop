/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Editor.h"
#include "QtPropertyGrid/QtPropertyGrid.h"

// temp for test
#include "Game/MACRO.h"
#include "Game/Component_CameraInterface.h"
#include "Game/Component_Render.h"
#include "Game/Component_Audio.h"

using namespace Teardrop;
using namespace Tools;

Editor::Editor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mPropGrid(0)
{
	ui.setupUi(this);
	mPropGrid = new QtPropertyGrid(ui.centralWidget);
	mPropGrid->clear();

	MACRO* macro = TD_NEW MACRO;
	macro->initialize();

	Component* comp = TD_NEW RenderComponent;
	comp->initialize();
	macro->addComponent(comp);
	comp = TD_NEW AudioComponent;
	comp->initialize();
	macro->addComponent(comp);
	comp = TD_NEW CameraInterfaceComponent;
	comp->initialize();
	macro->addComponent(comp);

	mPropGrid->setObject(macro);
}

Editor::~Editor()
{

}
