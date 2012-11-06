/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "SerialTaskScheduler.h"
#include "Task.h"
#include "TaskGraph.h"

using namespace Teardrop;
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
