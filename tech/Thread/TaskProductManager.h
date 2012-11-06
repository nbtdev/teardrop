/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TASKPRODUCTMANAGER_INCLUDED)
#define TASKPRODUCTMANAGER_INCLUDED

#include <Thread/TaskTypes.h>

// this interface is implemented by a TaskProduct "sink"
namespace Teardrop
{
	class TaskProduct;

	class TaskProductManager
	{
	public:
		// indicate availability of a TaskProduct instance to the TaskProductManager;
		// TaskProductManager does *not* take ownership of the product instance (it is
		// expected that in many cases, this simply will be a pointer to data that is 
		// owned by some other object or system in the application)
		virtual void productAvailable(TaskProduct*) = 0;

		// fetch a product instance by its type
		virtual const TaskProduct* fetchProduct(TaskProductType* pType) = 0;

		// clear all available products
		virtual void clearProducts() = 0;
	};
}

#endif // TASKPRODUCTMANAGER_INCLUDED
