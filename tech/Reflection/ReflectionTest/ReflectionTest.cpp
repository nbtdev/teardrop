// ReflectionTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
COS_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
COS_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
COS_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
//---------------------------------------------------------------------------
class TestClass : public Object
{
public:
	TestClass();
	~TestClass();

	COS_CLASS(TestClass, Object);
	COS_CLASS_CREATABLE();

	COS_PROPERTY(MyFloatProp, "My float property", float, -14, 0);
	COS_PROPERTY(MyIntProp, "My integer property", int, -29, 0);
	COS_PROPERTY(MyStringProp, "My string property", String, "Test String Val", 0);
	COS_PROPERTY(MyBoolProp, "My bool property", bool, false, 0);
};
//---------------------------------------------------------------------------
COS_CLASS_IMPL(TestClass);
//---------------------------------------------------------------------------
TestClass::TestClass()
{
}
//---------------------------------------------------------------------------
TestClass::~TestClass()
{
}
//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	TestClass test1;
	printf("Class '%s': size=%d\n", test1.getClassDef()->getName(), sizeof(test1));

	// get all props and print out info about them
	const PropertyDef* pProp = test1.getClassDef()->getProps();
	while (pProp)
	{
		printf("\tProperty: name='%s' type='%s'\n", pProp->getName(), pProp->getTypeName());
		pProp = pProp->m_pNext;
	}

	// find a specific property and set its value...
	int i=3, outInt;
	float f=22.7f, outFloat;
	bool b = true, outBool;
	String s("NNNNNNNN"), outString;
	pProp = test1.getClassDef()->findProperty("MyIntProp");
	assert(pProp);

	// ... first from binary....
	pProp->setData(&test1, &i);
	printf("\tProperty: name='%s', value='%d'\n", pProp->getName(), test1.getMyIntProp());

	// ...then from string
	pProp->setDataFromString(&test1, "9");
	pProp->getData(&test1, &outInt);
	printf("\tProperty: name='%s', value='%d'\n", pProp->getName(), outInt);

	// then do the same with the other props
	pProp = test1.getClassDef()->findProperty("MyFloatProp");
	assert(pProp);
	pProp->setData(&test1, &f);
	printf("\tProperty: name='%s', value='%f'\n", pProp->getName(), test1.getMyFloatProp());
	pProp->setDataFromString(&test1, "-3.1415927");
	pProp->getData(&test1, &outFloat);
	printf("\tProperty: name='%s', value='%f'\n", pProp->getName(), outFloat);

	pProp = test1.getClassDef()->findProperty("MyBoolProp");
	assert(pProp);
	pProp->setData(&test1, &b);
	printf("\tProperty: name='%s', value='%d'\n", pProp->getName(), test1.getMyBoolProp());
	pProp->setDataFromString(&test1, "0");
	pProp->getData(&test1, &outBool);
	printf("\tProperty: name='%s', value='%d'\n", pProp->getName(), outBool);

	pProp = test1.getClassDef()->findProperty("MyStringProp");
	assert(pProp);
	pProp->setData(&test1, &s);
	printf("\tProperty: name='%s', value='%s'\n", pProp->getName(), test1.getMyStringProp());
	pProp->setDataFromString(&test1, "WWWWWWWW");
	pProp->getData(&test1, &outString);
	printf("\tProperty: name='%s', value='%s'\n", pProp->getName(), (const char*)outString);

	return 0;
}

