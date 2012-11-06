/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "GameState.h"
#include "InputFilter.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
GameState::GameState()
{
}
//---------------------------------------------------------------------------
GameState::~GameState()
{
}
//---------------------------------------------------------------------------
bool GameState::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GameState::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool GameState::update()
{
	return true;
}
//---------------------------------------------------------------------------
bool GameState::filterInputEvent(const InputEvent& event)
{
	for (InputFilters::iterator i = m_inputFilters.begin();
		i != m_inputFilters.end(); ++i)
	{
		InputFilter* pFilter = *i;
		if (pFilter && pFilter->isEnabled())
		{
			if (pFilter->filterEvent(event))
			{
				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool GameState::addInputFilter(InputFilter& filter)
{
	m_inputFilters.push_back(&filter);
	return true; // could return false if filter already exists?
}
//---------------------------------------------------------------------------
InputFilter* GameState::removeInputFilter(InputFilter& filter)
{
	for (InputFilters::iterator i = m_inputFilters.begin();
		i != m_inputFilters.end(); ++i)
	{
		InputFilter* pFilter = *i;
		if (pFilter == &filter)
		{
			m_inputFilters.erase(i);
			return pFilter;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
void GameState::clearInputFilters()
{
	m_inputFilters.clear();
}
