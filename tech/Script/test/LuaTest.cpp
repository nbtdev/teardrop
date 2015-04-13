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
#include "Memory/Memory.h"
#include "Memory/MemoryRegion.h"
#include <assert.h>

using namespace CoS;

const static size_t HEAPSIZE = 1024 * 1024 * 256; // default to 256MB heap
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, HEAPSIZE);
COS_ALLOCATOR_IMPL(DEFAULT, DefaultAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(UNTRACKED, UntrackedAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)

class TestClass : public ScriptObject
{
	int m_a;
	const char* m_b;

public:
	TestClass(int a, const char* b) : m_a(a), m_b(b) {}
	~TestClass(){}

	void luaMethod(
		/*in*/ int i, float f, const char* c, bool b,
		/*out*/ int& oi, const char*& oc)
	{
		oi = m_a;
		oc = m_b;
	}
};

void TestCFunction3Args2Rtn(Variant* pArgs, int* argCount)
{
	assert(argCount && *argCount == 3);

	bool bVal = false;
	int iVal = 0;
	float fVal = 0;
	String sVal;
	void* pVal = 0;

	for (int i=0; i<*argCount; ++i)
	{
		switch (pArgs[i].type)
		{
		case Variant::VT_BOOL:
			bVal = pArgs[i].v.b;
			break;
		case Variant::VT_FLOAT:
			fVal = pArgs[i].v.f;
			break;
		case Variant::VT_STRING:
			sVal = pArgs[i].v.s;
			break;
		case Variant::VT_INT:
			iVal = pArgs[i].v.i;
			break;
		case Variant::VT_USER:
			pVal = pArgs[i].v.p;
			break;
		}
	}

	// send something back, 2 args
	*argCount = 2;
	pArgs[0].setFloat(1.2345f);
	pArgs[1].setString("Some String, eh?");
}

int main(int argc, char* argv[])
{
	String::setAllocator(GetDEFAULTAllocator());
	ScriptVM* vm = ScriptVM::create();

	vm->load("my.lua");
	vm->registerFunc("TestCFunction3Args2Rtn", TestCFunction3Args2Rtn);

	Variant args[3];
	args[0].setBool(true);
	args[1].setFloat(15.4156f);
	args[2].setString("Hello Script");
	vm->call("testCall", args, 3, 2);

	assert(1.2345f == args[0].v.f);
	assert(String("Some String, eh?") == args[1].v.s);

	ScriptVM::destroy(vm);

	return 0;
}

