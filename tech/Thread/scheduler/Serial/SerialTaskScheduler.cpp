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

#include "SerialTaskScheduler.h"
#include "Task.h"
#include "TaskGraph.h"

using namespace CoS;
//---------------------------------------------------------------------------
SerialTaskScheduler::SerialTaskScheduler()
{
}
//---------------------------------------------------------------------------
SerialTaskScheduler::~SerialTaskScheduler()
{
	delete m_pGraph;
}
//---------------------------------------------------------------------------
void SerialTaskScheduler::addTask(Task *pTask)
{
	m_pGraph->addTask(pTask);
}
//---------------------------------------------------------------------------
void SerialTaskScheduler::removeTask(Task *pTask)
{
	m_pGraph->removeTask(pTask);
}
//---------------------------------------------------------------------------
int SerialTaskScheduler::executeTasks()
{
	// this only does a sort if needed; otherwise, it simply returns a reference
	// to its cached list
	const TaskList& list = m_pGraph->topologicalSort();

	// then execute them all in a row
	int numTasks = 0;
	for (TaskList::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->execute(*this);
		++numTasks;
	}
	
	return numTasks;
}
