/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EDITING_INPUTFILTER_INCLUDED)
#define EDITING_INPUTFILTER_INCLUDED

#include "Game/InputFilter.h"
#include <list>

class InputFilter : public Teardrop::InputFilter
{
public:
	InputFilter();
	~InputFilter();

	enum ActionType
	{
		MOUSE,
		KEY,
	};

	struct Action
	{
		ActionType type;
		int extra[10];
		bool isInWindow;
	};

	bool filterEvent(const Teardrop::InputEvent& event);
	bool getNextAction(Action& type);

private:
	typedef std::list<Action> Actions;
	Actions m_actions;
};

#endif // EDITING_INPUTFILTER_INCLUDED
