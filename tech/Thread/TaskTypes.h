/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TASKTYPES_INCLUDED)
#define TASKTYPES_INCLUDED

#include <deque>

namespace Teardrop
{
	class Task;
	class TaskProduct;
	class TaskProductManager;

	namespace Reflection
	{
		class ClassDef;
	}

	typedef Reflection::ClassDef TaskProductType;
	typedef std::deque<TaskProductType*> ConsumesList;
	typedef std::deque<TaskProductType*> ProducesList;
	typedef std::deque<TaskProduct*> ProductList;
	typedef std::deque<Task*> TaskList;

}

#endif // TASKTYPES_INCLUDED
