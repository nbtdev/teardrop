/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TASKPRODUCT_INCLUDED)
#define TASKPRODUCT_INCLUDED

#include "Reflection/Reflection.h"

// derive from this class to implement data types for task products
namespace Teardrop
{
	class Task;

	class TaskProduct : public Reflection::Object
	{
	public:
		TD_CLASS(TaskProduct, Object);

		TaskProduct();
		TaskProduct(Task* pProducer);
		virtual ~TaskProduct();

		Task* getProducer() { return m_pProducer; }

	protected:
		Task *m_pProducer;
	};
}

#endif // TASKPRODUCT_INCLUDED
