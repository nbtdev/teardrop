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
#include "Util/Logger.h"
#include "Util/Environment.h"
#include "Util/FileSystem.h"
#include "Stream/FileStream.h"
#include "Gfx/GfxCommon.h"

// hacky
#include "Game/LandscapeScene.h"
static Teardrop::LandscapeScene __lscene;
// end hacky

int main(int argc, char *argv[])
{
	Teardrop::String::setAllocator(GetCRTAllocator());

	Teardrop::FileStream logStrm;
	Teardrop::Logger logger(logStrm);

	Teardrop::String logPath;
	Teardrop::FileSystem::getAppDataPath(logPath);
	logPath.append('/');
	logPath.append("TeardropEditor.log");

	if (!logStrm.open(logPath, Teardrop::WRITE|Teardrop::TRUNCATE|Teardrop::TEXT))
	{
		// todo: let the user know
		return -1;
	}

	Teardrop::Environment& env = Teardrop::Environment::get();
	env.pLogger = &logger;
	env.isOffline = false;

	Teardrop::setGfxAllocator(GetDEFAULTAllocator());
	Teardrop::GfxInit();

	QApplication a(argc, argv);
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
