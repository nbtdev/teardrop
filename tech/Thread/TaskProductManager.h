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
