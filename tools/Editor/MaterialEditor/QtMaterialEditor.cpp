/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtMaterialEditor.h"
#include "QtPropertyGrid/QtPropertyGrid.h"
#include "Gfx/Material.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QListView>

using namespace Teardrop;
using namespace Tools;

QtMaterialEditor::QtMaterialEditor(Gfx::Material* mtl, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mMaterial(mtl)
{
	QVBoxLayout* vlayout = new QVBoxLayout(this);
	QWidget* top = new QWidget(this);
	QWidget* bottom = new QWidget(this);
	vlayout->addWidget(top);
	vlayout->addWidget(bottom);

	QHBoxLayout* hlayout = new QHBoxLayout(this);
	m3DView = new QWidget(this);
	mView = new QGraphicsView(this);
	mExpressionListView = new QListView(this);
	hlayout->addWidget(m3DView);
	hlayout->addWidget(mView);
	hlayout->addWidget(mExpressionListView);
}

QtMaterialEditor::~QtMaterialEditor()
{

}
