/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMESTATEMANAGER_INCLUDED)
#define GAMESTATEMANAGER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class GameState;
	struct Environment;

	class GameStateManager
	{
		GameState* m_pCurrentState;
		GameState* m_pPendingState;

	public:
		GameStateManager();
		~GameStateManager();

		bool initialize();
		bool destroy();

		bool changeState(GameState* pNextState);
		bool update();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // GAMESTATEMANAGER_INCLUDED
