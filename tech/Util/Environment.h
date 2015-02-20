/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
		Teardrop::SystemManager*		pSystemMgr;

		Environment();
		static Environment& get();
	};
}

#endif // TDENVIRONMENT_INCLUDED
