/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Bone.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Bone::Bone()
{
}
//---------------------------------------------------------------------------
Bone::~Bone()
{
}
//---------------------------------------------------------------------------
bool Bone::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Bone::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
const char* Bone::getName() const
{
	return 0;
}
//---------------------------------------------------------------------------
Bone* Bone::getParent(const char* /*name*/) const
{
	return 0;
}
//---------------------------------------------------------------------------
size_t Bone::getNumChildren() const
{
	return 0;
}
//---------------------------------------------------------------------------
const Bone* Bone::getChild(size_t /*index*/) const
{
	return 0;
}
//---------------------------------------------------------------------------
Bone* Bone::getChild(size_t /*index*/)
{
	return 0;
}
