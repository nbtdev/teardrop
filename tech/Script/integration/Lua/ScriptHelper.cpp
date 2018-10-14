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

#include "ScriptHelper.h"
#include "Variant.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ScriptHelper::ScriptHelper(lua_State& L) : m_l(L)
{
}
//---------------------------------------------------------------------------
bool ScriptHelper::getParam(int idx, Variant& param)
{
	if (idx < 1 || idx > lua_gettop(&m_l))
	{
		param.type = Variant::VT_NONE;
		return false;
	}

	if (lua_isboolean(&m_l, idx))
	{
		param.type = Variant::VT_BOOL;
		param.v.b = (lua_toboolean(&m_l, idx) == 1);
		return true;
	}
	
	if (lua_isnumber(&m_l, idx))
	{
		if (param.type == Variant::VT_INT)
		{
			param.v.i = (int)lua_tointeger(&m_l, idx);
		}
		else
		{
			param.v.f = (float)lua_tonumber(&m_l, idx);
		}
		return true;
	}
	
	if (lua_isstring(&m_l, idx))
	{
		param.type = Variant::VT_STRING;
		param.v.s = lua_tostring(&m_l, idx);
		return true;
	}
	
	if (lua_islightuserdata(&m_l, idx))
	{
		param.type = Variant::VT_USER;
		param.v.p = const_cast<void*>(lua_topointer(&m_l, idx));
		return true;
	}

	if (lua_isuserdata(&m_l, idx))
	{
		param.type = Variant::VT_USER;
		param.v.p = const_cast<void*>(lua_topointer(&m_l, idx));
		return true;
	}

	param.type = Variant::VT_NONE;
	return false;
}
//---------------------------------------------------------------------------
bool ScriptHelper::setParam(int idx, Variant& param)
{
	switch(param.type)
	{
	case Variant::VT_BOOL:
		lua_pushboolean(&m_l, param.v.b ? 1 : 0);
		break;

	case Variant::VT_INT:
		lua_pushinteger(&m_l, param.v.i);
		break;

	case Variant::VT_FLOAT:
		lua_pushnumber(&m_l, (lua_Number)param.v.f);
		break;

	case Variant::VT_STRING:
		lua_pushstring(&m_l, param.v.s);
		break;

	case Variant::VT_USER:
		lua_pushlightuserdata(&m_l, param.v.p);
		break;

	default:
		lua_pushnil(&m_l);
		break;
	}
	return true;
}
//---------------------------------------------------------------------------
LuaScriptVM* ScriptHelper::getCurrentVM()
{
	// this is stored in the globals 
	lua_getglobal(&m_l, "CURRENTVM");
	return (LuaScriptVM*)lua_topointer(&m_l, lua_gettop(&m_l));
}
