/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

//#include "stdafx.h"

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "LuaScriptVM.h"
#include "ScriptHelper.h"
#include "Variant.h"
#include "stdlib.h"
#include "stdarg.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
//#define LUA_YIELD	1
//#define LUA_ERRRUN	2
//#define LUA_ERRSYNTAX	3
//#define LUA_ERRMEM	4
//#define LUA_ERRERR	5
static bool isError(int code)
{
	switch (code)
	{
	case LUA_YIELD:
	case 0:
		return false;

	default:
		return true;
	}
}
//---------------------------------------------------------------------------
#define FAILED(e) (isError(e))
//---------------------------------------------------------------------------
void pushValue(lua_State* L, Variant* val)
{
	// set stack top to NIL if val is also null
	if (!val)
	{
		lua_pushnil(L);
	}
	else
	{
		switch(val->type)
		{
		case Variant::VT_BOOL:
			lua_pushboolean(L, (val->v.b ? 1 : 0));
			break;

		case Variant::VT_FLOAT:
			lua_pushnumber(L, (lua_Number)val->v.f);
			break;

		case Variant::VT_INT:
			lua_pushinteger(L, (lua_Integer)val->v.i);
			break;

		case Variant::VT_NONE:
			lua_pushnil(L);
			break;

		case Variant::VT_STRING:
			lua_pushstring(L, val->v.s);
			break;

		case Variant::VT_USER:
			if (val->v.p)
			{
				lua_pushlightuserdata(L, val->v.p);
			}
			else
			{
				lua_pushnil(L);
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------
LuaScriptVM::LuaScriptVM()
{
	m_pL = 0;
}
//---------------------------------------------------------------------------
LuaScriptVM::~LuaScriptVM()
{
}
//---------------------------------------------------------------------------
bool LuaScriptVM::initialize()
{
	m_pL = lua_open();
	luaL_openlibs(m_pL);

	// push us into the globals for use by free- and static-functions
	lua_pushlightuserdata(m_pL, this);
	lua_setglobal(m_pL, "CURRENTVM");

	return true;
}
//---------------------------------------------------------------------------
void LuaScriptVM::destroy()
{
	// clean up all script objects that remain
	for(ScriptObjects::iterator it = m_objects.begin(); 
		it != m_objects.end(); ++it)
	{
		delete *it;
	}

	m_objects.clear();

	if (m_pL)
	{
		lua_close(m_pL);
	}

	m_pL = 0;
}
//---------------------------------------------------------------------------
int LuaScriptVM::scriptCallback(lua_State* L)
{
	// pull out all of the arguments provided by the script; limited to 16
	// arguments for now
	ScriptHelper h(*L);
	Variant args[16];
	int argCount = 0;
	
	while (h.getParam(argCount+1, args[argCount])) ++argCount;

	// then look up the fn pointer to invoke the call
	int idx = lua_upvalueindex(1);
	scriptFn fn = (scriptFn)lua_topointer(L, idx);
	fn(args, &argCount);

	// push any return values 
	for (int i=0; i<argCount; ++i)
	{
		h.setParam(i, args[i]);
	}

	return argCount; // num returns
}
//---------------------------------------------------------------------------
bool LuaScriptVM::registerFunc(const char* scriptName, scriptFn fn)
{
	if (m_pL)
	{
		// register with Lua
        lua_pushlightuserdata(m_pL, (void*)fn); // store the original function pointer
		lua_pushcclosure(m_pL, scriptCallback, 1);
		lua_setglobal(m_pL, scriptName);
	}

	// and then register with us

	return true;
}
//---------------------------------------------------------------------------
bool LuaScriptVM::load(const char *moduleName)
{
	if (LUA_ERRSYNTAX == luaL_loadfile(m_pL, moduleName))
	{
		const char* errMsg = (const char*)lua_tostring(m_pL, -1);
		printf("%s\n", errMsg);
		return false;
	}

	lua_pcall(m_pL, 0, LUA_MULTRET, 0);
	return true;
}
//---------------------------------------------------------------------------
void LuaScriptVM::call(const char* fn, 
	Variant* args, // args to the script fn, return vals from the script fn
	size_t argCount, // num args into script
	size_t outCount // num return values expected
	)
{
	if (!m_pL)
	{
		return;
	}

	lua_getglobal(m_pL, fn);

	ScriptHelper h(*m_pL);
	if (argCount)
	{
		for (size_t i=0; i<argCount; ++i)
		{
			h.setParam(i, args[i]);
		}
	}

	if (LUA_ERRRUN == lua_pcall(m_pL, (int)argCount, (int)outCount, 0))
	{
		const char* errMsg = (const char*)lua_tostring(m_pL, -1);
		printf("%s\n", errMsg);
	}

	// process return values
	if (args)
	{
		args[0].type = Variant::VT_NONE;
	}

	int rtnIdx = lua_gettop(m_pL);
	for (int i=0; i<(int)outCount; ++i)
	{
		h.getParam(i+1, args[i]);
	}
}
//---------------------------------------------------------------------------
bool LuaScriptVM::setGlobal(const char* name, Variant* val)
{
	if (!m_pL)
	{
		return false;
	}

	pushValue(m_pL, val);
	lua_setglobal(m_pL, name);
	return true;
}
//---------------------------------------------------------------------------
bool LuaScriptVM::setNVP(const char* name, Variant* table, Variant* value)
{
	if (!m_pL || !table)
	{
		return false;
	}

	//push table
	pushValue(m_pL, table);

	// push value
	pushValue(m_pL, value);

	lua_setfield(m_pL, -2, name);
	return true;
}
//---------------------------------------------------------------------------
void LuaScriptVM::addScriptObject(ScriptObject* pObj)
{
	m_objects.push_back(pObj);
}
//---------------------------------------------------------------------------
void LuaScriptVM::destroyScriptObject(ScriptObject* pObj)
{
	for (ScriptObjects::iterator it = m_objects.begin(); 
		it != m_objects.end(); ++it)
	{
		if (*it == pObj)
		{
			delete *it;
			return;
		}
	}
}
