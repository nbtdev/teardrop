/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TaskProduct.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(TaskProduct);
//---------------------------------------------------------------------------
TaskProduct::TaskProduct()
{
}
//---------------------------------------------------------------------------
TaskProduct::TaskProduct(Task* pProducer)
	: m_pProducer(pProducer)
	, Object()
{
}
//---------------------------------------------------------------------------
TaskProduct::~TaskProduct()
{
}
