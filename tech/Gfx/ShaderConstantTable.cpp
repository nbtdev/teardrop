/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ShaderConstantTable.h"
#include "ShaderConstant.h"

using namespace Teardrop;
using namespace Gfx;

ShaderConstantTable::ShaderConstantTable()
{

}

ShaderConstantTable::~ShaderConstantTable()
{
	for (ShaderConstants::iterator it = mConstants.begin(); it != mConstants.end(); ++it) {
		delete it->second;
	}
}

ShaderConstant* ShaderConstantTable::addNew(const String& name, VertexElementType type, int width, int rows)
{
	ShaderConstant* constant = TD_NEW ShaderConstant(type, width, rows);
	std::pair<ShaderConstants::iterator, bool> pr = mConstants.insert(ShaderConstants::value_type(name, constant));
	if (!pr.second) {
		// already exists, return the existing one and delete the one we new'ed
		delete constant;
		constant = pr.first->second;
	}
	else {
		ConstantAdded.raise(name, constant);
	}

	return constant;
}

ShaderConstant* ShaderConstantTable::find(const String& name)
{
	ShaderConstants::iterator it = mConstants.find(name);
	if (it != mConstants.end()) {
		return it->second;
	}

	return 0;
}
