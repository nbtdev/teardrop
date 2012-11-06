/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TASKSCHEDULER_INCLUDED)
#define TASKSCHEDULER_INCLUDED

#include "TaskProductManager.h"
#include "Memory/Allocators.h"
#include <map>

namespace Teardrop
{
	class Task;
	class TaskGraph;

	class TaskScheduler
		: public TaskProductManager
	{
	public:
		TaskScheduler();
		virtual ~TaskScheduler();

		// to be implemented by derived classes
		virtual void addTask(Task *) = 0;
		virtual void removeTask(Task *) = 0;
		// returns the number of Task instances executed
		virtual int executeTasks() = 0;

		enum SchedulerType
		{
			TASKSCHEDULER_SERIAL,
			TASKSCHEDULER_MT,
		};

		// static methods
		static TaskScheduler* create(SchedulerType type);
		static void destroy(TaskScheduler*);

		// TaskProductManager implementation
		void productAvailable(TaskProduct* pProduct);
		TaskProduct* fetchProduct(TaskProductType* pType);
		void clearProducts();

		TD_DECLARE_ALLOCATOR();

	protected:
		TaskGraph* m_pGraph;

		typedef std::map<TaskProductType*, ProductList> ProductLUT;
		ProductLUT m_productLUT;
	};
}

#endif // TASKSCHEDULER_INCLUDED
