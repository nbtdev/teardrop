/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Editor.h"
#include <QtGui/QApplication>
#include <QtGui/QPalette>

#if defined(TD_OPTION_MEMPROFILE)
#include "Memory/AllocationTracker.h"
#endif

#include "Memory/Allocators.h"
#include "Memory/MemoryRegion.h"
#include "Util/_String.h"

int main(int argc, char *argv[])
{
	Teardrop::String::setAllocator(GetCRTAllocator());

	QApplication a(argc, argv);
	//QPalette palette;
	//palette.setColor(QPalette::Dark, QColor(Qt::darkBlue));
	//a.setPalette(palette);
	Teardrop::Tools::Editor w;
	w.show();
	return a.exec();
}

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
TD_ALLOCATOR_IMPL(DEFAULT, DefaultAllocator, DEFAULT, size_t(-1), size_t(-1))
TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, size_t(-1), size_t(-1))
//---------------------------------------------------------------------------