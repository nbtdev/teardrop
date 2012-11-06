/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TaskScheduler.h"
#include "TaskGraph.h"
#include "TaskProduct.h"
#include "CnC\CnCTaskScheduler.h"
#include "Serial\SerialTaskScheduler.h"
#include <algorithm>

using namespace Teardrop;
//---------------------------------------------------------------------------
TaskScheduler::TaskScheduler()
{
	m_pGraph = TD_NEW TaskGraph;
}
//---------------------------------------------------------------------------
TaskScheduler::~TaskScheduler()
{
}
//---------------------------------------------------------------------------
TaskScheduler* TaskScheduler::create(SchedulerType type)
{
	switch (type)
	{
	case TASKSCHEDULER_SERIAL:
		return TD_NEW SerialTaskScheduler();
	case TASKSCHEDULER_MT:
		return TD_NEW CnCTaskScheduler();
	}

	return 0;
}
//---------------------------------------------------------------------------
void TaskScheduler::destroy(TaskScheduler* pSched)
{
	delete pSched;
}
//---------------------------------------------------------------------------
void TaskScheduler::productAvailable(TaskProduct* pProduct)
{
	std::pair<ProductLUT::iterator, bool> pr = m_productLUT.insert(
		ProductLUT::value_type(pProduct->getDerivedClassDef(), ProductList()));
	pr.first->second.push_back(pProduct);
}
//---------------------------------------------------------------------------
TaskProduct* TaskScheduler::fetchProduct(TaskProductType* pType)
{
	ProductLUT::iterator it = m_productLUT.find(pType);
	if (it == m_productLUT.end()) 
		return 0;
	if (!it->second.size())
		return 0;

	TaskProduct* rtn = it->second.front();
	//it->second.pop_front(); // TODO: multiple tasks may want this data
	return rtn;
}
//---------------------------------------------------------------------------
void TaskScheduler::clearProducts()
{
	for (ProductLUT::iterator it = m_productLUT.begin(); 
		it != m_productLUT.end(); ++it)
	{
		it->second.clear();
	}
}
