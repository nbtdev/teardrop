/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMESTATE_INCLUDED)
#define GAMESTATE_INCLUDED

#include <list>
#include "Memory/Allocators.h"

namespace Teardrop
{
	struct Environment;
	union InputEvent;
	class InputFilter;

	class GameState
	{
		typedef std::list<InputFilter*> InputFilters;
		InputFilters m_inputFilters;

	public:
		GameState();
		virtual ~GameState();

		virtual bool initialize();
		virtual bool destroy();
		virtual bool update();

		bool filterInputEvent(const InputEvent& event);
		bool addInputFilter(InputFilter& pFilter);
		InputFilter* removeInputFilter(InputFilter& pFilter);
		void clearInputFilters();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // GAMESTATE_INCLUDED
