/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCRIPTVM_INCLUDED)
#define SCRIPTVM_INCLUDED

#include <list>

namespace Teardrop
{
	struct Variant;
	// Variant* is "in" args and "out" returns; int is the number in each case
	typedef void (*scriptFn)(Variant*, int*);

	class ScriptObject
	{
	public:
		ScriptObject();
		virtual ~ScriptObject();
	};
	typedef std::list<ScriptObject*> ScriptObjects;

	class ScriptVM
	{
	public:
		ScriptVM();
		virtual ~ScriptVM();

		static ScriptVM* create();
		static void destroy(ScriptVM*);

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		// load a script module (either text or compiled)
		virtual bool load(const char* moduleName) = 0;
		// used to call into script from C++
		virtual void call(
			const char* fn,			// script function name 
			Variant* args=0,		// script function args, also contains function return values 
			size_t argCount=0,		// num args to the script
			size_t outCount=0		// num return values expected
			) = 0;	
		// register a function that can be called from script
		virtual bool registerFunc(const char* luaName, scriptFn fn) = 0;
		// set a global variable accessible by the script
		virtual bool setGlobal(const char* name, Variant* val) = 0;
		// set a name/value pair accessible by the script
		virtual bool setNVP(const char* name, Variant* table, Variant* value) = 0;

		virtual void addScriptObject(ScriptObject* pObj) = 0;
		virtual void destroyScriptObject(ScriptObject* pObj) = 0;
	};
}

#endif // SCRIPTVM_INCLUDED
