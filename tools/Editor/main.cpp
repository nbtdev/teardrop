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

#include "Editor.h"
#include <QtWidgets/QApplication>
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

#if defined(_WIN32) || defined(_WIN64)
    #include "Direct3D11/Integration.h"
//    #include "Direct3D9/Integration.h"
//    #include "DirectInput8/Integration.h"
	#include "OpenGL/IntegrationOpenGL.h"
#else // _WIN32, _WIN64
    #include "OpenGL/IntegrationOpenGL.h"
    #include "XWindow/Integration.h"
#endif // _WIN32, _WIN64

// hacky
#include "Game/LandscapeScene.h"
static Teardrop::LandscapeScene __lscene;
// end hacky

int main(int argc, char *argv[])
{
	Teardrop::String::setAllocator(GetCRTAllocator());

	Teardrop::FileStream logStrm;
	Teardrop::Logger logger(logStrm);

    Teardrop::String logPath, logFilePath;
    Teardrop::FileSystem::getAppDataPath(logPath);
    Teardrop::FileSystem::getAppDataPath(logFilePath);
    logFilePath.append('/');
    logFilePath.append("TeardropEditor.log");

    if (!logStrm.open(logFilePath, Teardrop::WRITE|Teardrop::TRUNCATE|Teardrop::TEXT))
	{
        // attempt to create the directory
        if (Teardrop::FileSystem::createDirectory(logPath)) {
            // todo: let the user know
            return -2;
        }

        if (!logStrm.open(logFilePath, Teardrop::WRITE|Teardrop::TRUNCATE|Teardrop::TEXT)) {
            // todo: let the user know
            return -1;
        }
	}

	Teardrop::Environment& env = Teardrop::Environment::get();
	env.pLogger = &logger;
	env.isOffline = false;

	// this is a bit hacky?
#if defined(_WIN32) || defined(_WIN64)
	//Teardrop::Gfx::Direct3D9::registerIntegration();
	Teardrop::Gfx::Direct3D11::registerIntegration();
	Teardrop::Gfx::OpenGL::registerIntegration();
	//Teardrop::DirectInput::Integration inputIntegration;
#else // _WIN32, _WIN64
    Teardrop::Gfx::OpenGL::registerIntegration();
    Teardrop::XWindow::Integration inputIntegration;
#endif // _WIN32, _WIN64

	try {
		QApplication a(argc, argv);
		Teardrop::Tools::Editor w;
		w.show();
		return a.exec();
	} catch (...) {
		return -1;
	}
}

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
TD_ALLOCATOR_IMPL(DEFAULT, DefaultAllocator, DEFAULT, size_t(-1), size_t(-1))
TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, size_t(-1), size_t(-1))
//---------------------------------------------------------------------------
