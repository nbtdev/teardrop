/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COSENVIRONMENT_INCLUDED)
#define COSENVIRONMENT_INCLUDED

struct GameOptions;

namespace CoS
{
	namespace Audio
	{
		class Renderer;
	}

	class GfxRenderer;
	class GfxCommandBuffer;
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
		GfxRenderer*		pRenderer;
		Audio::Renderer*	pAudio;
		GfxCommandBuffer*	pGfxCmdBuf;
		Logger*				pLogger;
		GameStateManager*	pGSM;
		Input*				pInput;
		Timer*				pMasterClock;
		ResourceManager*	pResourceMgr;
		Manifest*			pManifest;

		Environment();
		static Environment& get();
	};
}

#endif // COSENVIRONMENT_INCLUDED
