/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Lua/LuaScriptVM.h"

using namespace Teardrop; 
//---------------------------------------------------------------------------
ScriptVM* ScriptVM::create()
{
	ScriptVM* pVM = TD_NEW LuaScriptVM;
	pVM->initialize();
	return pVM;
}
//---------------------------------------------------------------------------
void ScriptVM::destroy(ScriptVM* pVM)
{
	if (pVM)
		pVM->destroy();

	delete pVM;
}
//---------------------------------------------------------------------------
ScriptVM::ScriptVM()
{
}
//---------------------------------------------------------------------------
ScriptVM::~ScriptVM()
{
}
