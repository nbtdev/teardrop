/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALPOINTERLIST_INCLUDED)
#define SERIALPOINTERLIST_INCLUDED

#include "Serialization/SerialPointer.h"

namespace Teardrop
{
	template<typename T>
	struct SerialPointerListElem
	{
		SerialPointer<T> pThis;
		SerialPointer<T> pNext;

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SERIALPOINTERLIST_INCLUDED
