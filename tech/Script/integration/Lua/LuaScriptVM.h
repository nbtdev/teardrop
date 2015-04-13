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
