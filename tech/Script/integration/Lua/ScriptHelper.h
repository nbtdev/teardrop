/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined (SCRIPTHELPER_INCLUDED)
#define SCRIPTHELPER_INCLUDED

#include "Memory/Memory.h"

struct lua_State;

namespace Teardrop
{
	struct Variant;
	class LuaScriptVM;

	struct ScriptHelper
	{
		lua_State& m_l;
		ScriptHelper(lua_State& L);

		bool getParam(int idx, Variant& param);
		bool setParam(int idx, Variant& param);
		LuaScriptVM* getCurrentVM();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SCRIPTHELPER_INCLUDED
