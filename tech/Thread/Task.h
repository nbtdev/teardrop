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

#if !defined(TASK_INCLUDED)
#define TASK_INCLUDED

#include "Thread/TaskTypes.h"

namespace CoS
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
