/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
