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

#include "CnCTaskScheduler.h"
#include "Task.h"
#include "TaskProduct.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include <cnc/cnc.h>
#include <vector>
#include <map>

using namespace Teardrop;
//---------------------------------------------------------------------------
struct Teardrop::cnc_context : public CnC::context<cnc_context>
{
	TD_DECLARE_ALLOCATOR();

	~cnc_context() {}
	cnc_context(TaskProductManager& tpm);

	CnC::tag_collection<size_t> m_tags;
	CnC::item_collection<TaskProductType*, TaskProduct*> m_items;

	typedef std::vector<Task*> Tasks;
	Tasks m_tasks;

	TaskProductManager& m_tpm;
};
//---------------------------------------------------------------------------
struct task_execute : public CnC::default_tuner<size_t, cnc_context>
{
	int execute(const size_t& tag, cnc_context& ctx) const
	{
		ctx.m_tasks[tag]->execute(ctx.m_tpm);
		return CnC::CNC_Success;
	}

	template<class DC>
	void depends(const size_t& tag, cnc_context& ctx, DC& dC) const
	{
		Task* pTask = ctx.m_tasks[tag];
		for (ConsumesList::const_iterator it = pTask->consumes().begin(); 
			it != pTask->consumes().end(); ++it)
		{
			dC.depends(ctx.m_items, *it);
		}
	}
};
//---------------------------------------------------------------------------
cnc_context::cnc_context(TaskProductManager& tpm) 
: CnC::context<cnc_context>()
, m_tpm(tpm)
, m_tags(this)
, m_items(this)
{
	prescribe(m_tags, task_execute(), task_execute());
}
//---------------------------------------------------------------------------
CnCTaskScheduler::CnCTaskScheduler()
{
	m_pContext = TD_NEW cnc_context(*this);
}
//---------------------------------------------------------------------------
CnCTaskScheduler::~CnCTaskScheduler()
{
	delete m_pContext;
}
//---------------------------------------------------------------------------
void CnCTaskScheduler::addTask(Task *pTask)
{
	m_pContext->m_tasks.push_back(pTask);
}
//---------------------------------------------------------------------------
void CnCTaskScheduler::removeTask(Task *pTask)
{
}
//---------------------------------------------------------------------------
int CnCTaskScheduler::executeTasks()
{
	// put all of the tags (1:1 with steps, which are 1:1 with Task*)
	for (size_t i=0; i<m_pContext->m_tasks.size(); ++i)
		m_pContext->m_tags.put(i);

	m_pContext->wait();

	return 0;
}
//---------------------------------------------------------------------------
void CnCTaskScheduler::productAvailable(TaskProduct* pProduct)
{
	// CnC item_collection is threadsafe, and where products need to go anyway
	m_pContext->m_items.put(pProduct->getDerivedClassDef(), pProduct);
}
//---------------------------------------------------------------------------
TaskProduct* CnCTaskScheduler::fetchProduct(TaskProductType* pType)
{
	TaskProduct* pProduct = 0;
	m_pContext->m_items.get(pType, pProduct);
	return pProduct;
}
//---------------------------------------------------------------------------
void CnCTaskScheduler::clearProducts()
{
	m_pContext->m_items.reset();
}
