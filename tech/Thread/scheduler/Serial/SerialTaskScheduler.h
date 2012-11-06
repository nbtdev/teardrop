/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALTASKSCHEDULER_INCLUDED)
#define SERIALTASKSCHEDULER_INCLUDED

#include "TaskScheduler.h"

namespace Teardrop
{
	class SerialTaskScheduler : public TaskScheduler
	{
	public:
		SerialTaskScheduler();
		virtual ~SerialTaskScheduler();

		// TaskScheduler implementation
		void addTask(Task *);
		void removeTask(Task *);
		int executeTasks();

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // SERIALTASKSCHEDULER_INCLUDED
