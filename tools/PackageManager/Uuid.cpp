/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Uuid.h"

using namespace Teardrop;
using namespace Tools;

#include "Util/_String.h"

#if defined(_WIN32) || defined(_WIN64)
#include <rpc.h>

void Teardrop::Tools::UuidGen(String& uuid) 
{
	uuid.clear();

	UUID uuidGen;
	if (RPC_S_OK == UuidCreate(&uuidGen)) {
		RPC_CSTR str;
		if (RPC_S_OK == UuidToString(&uuidGen, &str)) {
			uuid = (const char*)str;
		}
	}
}
#endif // Windows