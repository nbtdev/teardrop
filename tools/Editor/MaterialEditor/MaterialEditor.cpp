/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "MaterialEditor.h"
#include "EditorCanvas.h"
#include "ExpressionConnection.h"
#include "ExpressionConnectorDatabase.h"
#include "ExpressionItem.h"
#include "PropertyGrid/PropertyGrid.h"
#include "ProjectExplorer/ProjectItem.h"
#include "PackageManager/Metadata.h"
#include "Package/Package.h"

#include "Gfx/Connection.h"
#include "Gfx/Material.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>

#include <cmath>

namespace Teardrop {
namespace Tools {

MaterialEditor::MaterialEditor(ProjectItem* materialItem, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mPackage(0)
	, mState(IDLE)
{
	mMaterial = static_cast<Gfx::Material*>(materialItem->object());
	Metadata* meta = materialItem->metadata();

	mConnectors.reset(TD_NEW ExpressionConnectorDatabase);

	setWindowIcon(QIcon("icons/td-icon-32.png"));

	QVBoxLayout* vlayout = TD_NEW QVBoxLayout(this);
	QWidget* top = TD_NEW QWidget(this);
	mPropGrid = TD_NEW PropertyGrid(this);
	mPropGrid->setObject(mMaterial);
	vlayout->addWidget(top);
	vlayout->addWidget(mPropGrid);

	QHBoxLayout* hlayout = TD_NEW QHBoxLayout(top);
	m3DView = TD_NEW QWidget(this);
	mView = TD_NEW EditorCanvas(mMaterial, this);
	hlayout->addWidget(m3DView);
	hlayout->addWidget(mView);

	QWidget* viewWidget = TD_NEW QWidget(this);
	QVBoxLayout* vlayout2 = TD_NEW QVBoxLayout(viewWidget);
	QLabel* label = TD_NEW QLabel(viewWidget);
	label->setText("Material Expressions");

	mExpressionListView = TD_NEW QListView(this);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mExpressionListView->sizePolicy().hasHeightForWidth());
	mExpressionListView->setSizePolicy(sizePolicy);

	vlayout2->addWidget(label);
	vlayout2->addWidget(mExpressionListView);

	hlayout->addWidget(viewWidget);

	setAttribute(Qt::WA_DeleteOnClose);
	resize(1280, 800);

	QString title("Material Editor - ");
	title.append(meta->getName());
	setWindowTitle(title);

	// populate the expression list with candidates
	QStringList exprNames;
	Reflection::ClassDef* classDef = Reflection::ClassDef::getClasses();
	Reflection::ClassDef* mtlExprClass = Gfx::MaterialExpression::getClassDef();

	while (classDef) {
		if (classDef->isA(mtlExprClass) && classDef != mtlExprClass && classDef->isCreatable()) {
			exprNames.append(classDef->getName());
			mExpressionClasses.push_back(classDef);
		}

		classDef = classDef->m_pNext;
	}

	mExpressionModel.setStringList(exprNames);
	mExpressionListView->setModel(&mExpressionModel);

	mView->setScene(TD_NEW QGraphicsScene);

	// add the expressions present in the material
	if (mMaterial) {
		// any material created in the tools will have at least a MaterialOutput
		assert(mMaterial->getOutput());

		mMaterial->sortExpressions();
		Gfx::MaterialExpression** exprs = mMaterial->sortedExpressions();
		int nExpr = mMaterial->expressionCount();
		mExpressionItems.resize(nExpr);

		// TODO: when the user makes changes to the material editor item layout, we should save 
		// the item positions off so that the layout can be loaded again later; for now, we just 
		// spread them out in a grid

		// figure out grid row count by taking square root of number of expressions; we will 
		// take the floor of that value for the number of rows, and the ceiling of that value
		// as the number of columns
        qreal sqr = std::sqrt(float(nExpr));
        int rows = int(std::floor(sqr));
        int cols = int(std::ceil(sqr));

		int k = 0;
		qreal x = 0.f;
		qreal y = 0.f;
		for (int j = 0; j < rows; ++j, y += 300.f) {
			for (int i = 0; i < cols; ++i, x -= 150.f) {
				Gfx::MaterialExpression* expr = exprs[k];
				ExpressionItem* item = TD_NEW ExpressionItem(expr, mConnectors.get());
				mExpressionItems[k] = item;
				mView->scene()->addItem(item);
				item->setPos(x, y);
				++k;
			}
		}

		// make the connections between connected attributes
		int nConnections = mMaterial->connectionCount();
		std::vector<Gfx::Connection*> connections(nConnections);
		mMaterial->connections(connections.data(), nConnections);

		for (auto c : connections) {
			const Gfx::Attribute* from = c->output();
			const Gfx::Attribute* to = c->input();

			ExpressionConnector::ConstRef fromConn = mConnectors->findConnectorByAttribute(from);
			ExpressionConnector::ConstRef toConn = mConnectors->findConnectorByAttribute(to);

			ExpressionConnector::ConstPtr fromPtr = fromConn.lock();
			ExpressionConnector::ConstPtr toPtr = toConn.lock();

			if (fromPtr && toPtr) {
				ExpressionConnection* conn = TD_NEW ExpressionConnection(fromPtr, toPtr);
				mView->scene()->addItem(conn);
				mConnections.push_back(conn);
			}
		}
	}

	mView->ItemSelected.bind(std::bind(&MaterialEditor::onItemSelected, this, std::placeholders::_1));
	mView->PathSelected.bind(std::bind(&MaterialEditor::onConnectionSelected, this, std::placeholders::_1));
	mView->SelectionCleared.bind(std::bind(&MaterialEditor::onSelectionCleared, this));
}

MaterialEditor::~MaterialEditor()
{
	delete mPackage;
}

void MaterialEditor::onClose()
{
	close();
}
Package* MaterialEditor::package()
{
	if (!mPackage) {
		mPackage = TD_NEW Package;
	}

	return mPackage;
}

void MaterialEditor::onItemSelected(ExpressionItem* aItem)
{
	mPropGrid->setObject(aItem->expression());
}

void MaterialEditor::onConnectionSelected(ExpressionConnection* aItem)
{
	mPropGrid->setObject(nullptr);
}

void MaterialEditor::onSelectionCleared()
{
	mPropGrid->setObject(mMaterial);
}

} // namespace Tools
} // namespace Teardrop
