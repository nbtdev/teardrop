/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPackageExplorer.h"
#include "PackageItem.h"
#include "FolderItem.h"

using namespace Teardrop;
using namespace Tools;

QtPackageExplorer::QtPackageExplorer(QWidget* parent)
	: QTreeWidget(parent)
{
}

QtPackageExplorer::~QtPackageExplorer()
{
	for (PackageItems::iterator it = mRootItems.begin(); it != mRootItems.end(); ++it) {
		delete *it;
	}
}

void QtPackageExplorer::addPackage(Package* package)
{
	PackageItem* pkgItem = new PackageItem(package);
	mRootItems.push_back(pkgItem);
	addTopLevelItem(pkgItem->root());
}

void QtPackageExplorer::removePackage(Package* package)
{
	for (PackageItems::iterator it = mRootItems.begin(); it != mRootItems.end(); ++it) {
		if ((*it)->package() == package) {
			delete *it;
			mRootItems.erase(it);
			break;
		}
	}
}
