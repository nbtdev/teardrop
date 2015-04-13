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
