/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CNCTASKSCHEDULER_INCLUDED)
#define CNCTASKSCHEDULER_INCLUDED

#include "TaskScheduler.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	struct cnc_context;

	class CnCTaskScheduler : public TaskScheduler
	{
	public:
		CnCTaskScheduler();
		virtual ~CnCTaskScheduler();

		// TaskScheduler implementation
		void addTask(Task *);
		void removeTask(Task *);
		int executeTasks();

		// TaskProductManager implementation
		void productAvailable(TaskProduct* pProduct);
		TaskProduct* fetchProduct(TaskProductType* pType);
		void clearProducts();

		TD_DECLARE_ALLOCATOR();

	private:
		cnc_context* m_pContext;
	};
}

#endif // CNCTASKSCHEDULER_INCLUDED
