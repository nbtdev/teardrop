/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TASK_INCLUDED)
#define TASK_INCLUDED

#include "Thread/TaskTypes.h"

namespace Teardrop
{
	class Task
	{
	public:
		Task();
		virtual ~Task();

		// execute this task; when finished, pass on the task's product(s)
		// to the provided TaskProductManager reference
		virtual void execute(TaskProductManager&) = 0;

		// provide a listing of the TaskProduct types provided by this task
		const ProducesList& provides() { return m_produces; }
		// provide a listing of the TaskProduct types required by this task
		const ConsumesList& consumes() { return m_consumes; }

	protected:

		// both should be filled in by derived classes to indicate what type(s)
		// of thing(s) they produce and consume
		ProducesList m_produces;
		ConsumesList m_consumes;
	};
}

#define TASK_DEPENDS(c) \
	m_consumes.push_back(c::getClassDef())

#define TASK_PRODUCES(c) \
	m_produces.push_back(c::getClassDef())


#endif // TASK_INCLUDED
