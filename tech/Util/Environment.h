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

#if !defined(TDENVIRONMENT_INCLUDED)
#define TDENVIRONMENT_INCLUDED

#include <cstddef>

struct GameOptions;

namespace Teardrop
{
	class SystemManager;
}

namespace Teardrop
{
	namespace Audio
	{
		class Renderer;
	}

    namespace Gfx {
        class Renderer;
    }

	class Logger;
	class Input;
	class GameStateManager;
	class ResourceManager;
	class Timer;
	class Manifest;

	// it's the singleton place we store all of our singletons...
	struct Environment
	{
		// refers to "toolchain or runtime", not network -- hack because I didn't foresee this...
		size_t				isOffline;
		size_t				isServer;
		size_t				useTaskScheduler;

		// paths
		const char*			pDataPath;
		const char*			pAppDataPath;

		// system instances
		GameOptions*		pOptions;
        Gfx::Renderer*		pRenderer;
		Audio::Renderer*	pAudio;
		Logger*				pLogger;
		GameStateManager*	pGSM;
		Input*				pInput;
		Timer*				pMasterClock;
		ResourceManager*	pResourceMgr;
		Manifest*			pManifest;
		Teardrop::SystemManager*		pSystemMgr;

		Environment();
		static Environment& get();
	};
}

#endif // TDENVIRONMENT_INCLUDED
