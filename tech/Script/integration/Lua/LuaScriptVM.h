/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LUASCRIPTVM_INCLUDED)
#define LUASCRIPTVM_INCLUDED

#include "ScriptVM.h"
#include "Util/_String.h"
#include "Memory/Memory.h"
#include <map>

struct lua_State;

namespace Teardrop
{
	class LuaScriptVM
		: public ScriptVM
	{
		lua_State* m_pL;
		ScriptObjects m_objects;
		typedef std::map<String, scriptFn> FnLUT;
		FnLUT m_lut;

	public:
		LuaScriptVM();
		~LuaScriptVM();

		bool initialize();
		void destroy();

		bool load(const char* moduleName);
		void call(
			const char* fn,			// script function name 
			Variant* args=0,		// script function args, also contains function return values 
			size_t argCount=0,		// num args to the script
			size_t outCount=0		// num return values expected
			);	
		bool registerFunc(const char* luaName, scriptFn fn);
		bool setGlobal(const char* name, Variant* val);
		bool setNVP(const char* name, Variant* table, Variant* value);

		void addScriptObject(ScriptObject* pObj);
		void destroyScriptObject(ScriptObject* pObj);

		static int scriptCallback(lua_State*);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // LUASCRIPTVM_INCLUDED
