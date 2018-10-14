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

#include "TaskGraph.h"
#include "Task.h"
#include "Memory/Memory.h"
#include <set>

using namespace Teardrop;
//---------------------------------------------------------------------------
struct Teardrop::TaskGraphNode
{
	Task *pTask;
	ConsumesList consumes;
	ProducesList produces;

	TD_DECLARE_ALLOCATOR();
};
//---------------------------------------------------------------------------
TaskGraph::TaskGraph()
: m_bSortNeeded(false)
{
}
//---------------------------------------------------------------------------
TaskGraph::~TaskGraph()
{
	clear();
}
//---------------------------------------------------------------------------
void TaskGraph::clear()
{
	// the only LUT that owns any pointers is the m_graphNodeLUT, so 
	// go through that and clean up the TaskGraphNode instances
	for (GraphNodeLUT::iterator it = m_graphNodeLUT.begin(); 
		it != m_graphNodeLUT.end(); ++it)
	{
		delete it->second;
	}

	m_bSortNeeded = true;
}
//---------------------------------------------------------------------------
void TaskGraph::reset()
{
	clear();
}
//---------------------------------------------------------------------------
void TaskGraph::addTask(Task* pTask)
{
	GraphNodeLUT::iterator it = m_graphNodeLUT.find(pTask);
	if (it == m_graphNodeLUT.end())
	{
		TaskGraphNode* pNode = TD_NEW TaskGraphNode;
		pNode->consumes = pTask->consumes();
		pNode->produces = pTask->provides();
		pNode->pTask = pTask;

		m_graphNodeLUT[pTask] = pNode;

		// make myself known to the consumer LUT
		for (ConsumesList::iterator c = pNode->consumes.begin();
			c != pNode->consumes.end(); ++c)
		{
			std::pair<DependentLUT::iterator, bool> pr = 
				m_dependentLUT.insert(DependentLUT::value_type(*c, TaskGraphNodeList()));
			pr.first->second.push_back(pNode);
		}

		// and then record that this task produces this(these) product(s)
		for (ProducesList::iterator p = pNode->produces.begin(); 
			p != pNode->produces.end(); ++p)
		{
			m_producerLUT[*p] = pNode;
		}
	}

	m_bSortNeeded = true;
}
//---------------------------------------------------------------------------
void TaskGraph::removeTask(Task *pTask)
{
	m_bSortNeeded = true;
}
//---------------------------------------------------------------------------
void TaskGraph::findPredecessors(Task *pTask, /*inout*/TaskList& predecessors)
{
	predecessors.clear();

	GraphNodeLUT::iterator it = m_graphNodeLUT.find(pTask);
	if (it != m_graphNodeLUT.end())
	{
		TaskGraphNode* pNode = it->second;

		// use the dependency list for this task to figure out which Tasks 
		// are directly connected to it ("connected" in a DAG context)
		for (ConsumesList::iterator c = pNode->consumes.begin(); 
			c != pNode->consumes.end(); ++c)
		{
			// look up what produces this to get a TaskGraphNode for it
			ProducerLUT::iterator p = m_producerLUT.find(*c);
			if (p != m_producerLUT.end())
			{
				TaskGraphNode* pProducer = p->second;
				predecessors.push_back(pProducer->pTask);
			}
		}
	}
}
//---------------------------------------------------------------------------
void TaskGraph::findDependents(Task *pTask, /*inout*/TaskList& dependents)
{
	dependents.clear();

	GraphNodeLUT::iterator it = m_graphNodeLUT.find(pTask);
	if (it != m_graphNodeLUT.end())
	{
		TaskGraphNode* pNode = it->second;

		// use the dependency list for this task to figure out which Tasks 
		// are directly connected to it ("connected" in a DAG context)
		for (ProducesList::iterator p = pNode->produces.begin(); 
			p != pNode->produces.end(); ++p)
		{
			// look up what consumes this to get a TaskGraphNode for it
			DependentLUT::iterator d = m_dependentLUT.find(*p);
			if (d != m_dependentLUT.end())
			{
				TaskGraphNodeList& consumers = d->second;
				for (TaskGraphNodeList::iterator c = consumers.begin();
					c != consumers.end(); ++c)
				{
					dependents.push_back((*c)->pTask);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
// depth-first topological sort, from 
// http://en.wikipedia.org/wiki/Topological_sorting
//---------------------------------------------------------------------------
typedef std::set<Task*> VisitedSet;
static void visit(TaskGraph* pGraph, Task* pTask, TaskList& cache, VisitedSet& visited)
{
	if (visited.find(pTask) != visited.end())
		return;

	visited.insert(pTask);

	TaskList p;
	pGraph->findPredecessors(pTask, p);
	for (TaskList::iterator it = p.begin(); it != p.end(); ++it)
	{
		visit(pGraph, *it, cache, visited);
	}

	// add the leaf node to the lists
	cache.push_back(pTask);
}
//---------------------------------------------------------------------------
const TaskList& TaskGraph::topologicalSort()
{
	// early-out if possible
	if (m_bSortNeeded)
	{
		m_sortedTaskList.clear();
		VisitedSet visited;
		for (GraphNodeLUT::iterator it = m_graphNodeLUT.begin(); 
			it != m_graphNodeLUT.end(); ++it)
		{
			visit(this, it->second->pTask, m_sortedTaskList, visited);
		}

		m_bSortNeeded = false;
	}

	return m_sortedTaskList;
}
