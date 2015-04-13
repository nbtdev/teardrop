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
