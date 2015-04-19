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

#include "Function.h"
#include "ShaderRealizer.h"
#include "Gfx/Exception.h"
#include "Gfx/Codegen/Common.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Codegen {

Function::Function(const String& aName)
	: mName(aName)
{

}

Function::~Function()
{

}

void Function::realize(ShaderRealizer& aRealizer) const
{
	std::vector<Argument const*> args(mArguments.size());
	int i = 0;

	for (auto& a : mArguments) {
		args[i] = a.get();
		++i;
	}

	aRealizer.insertFunctionPrologue(TYPE_VOID, mName, args.data(), int(args.size()));
	aRealizer.openScope();

	for (auto s : mStatements) {
		s->realize(aRealizer);
		aRealizer.endStatement();
	}

	aRealizer.closeScope();
}

void Function::addArgument(Argument::ConstPtr aArg)
{
	mArguments.push_back(aArg);
}

void Function::addStatement(Expression::ConstPtr aStatement)
{
	mStatements.push_back(aStatement);
}

const String& Function::name() const
{
	return mName;
}

unsigned int Function::argumentCount() const
{
	return int(mArguments.size());
}

unsigned int Function::statementCount() const
{
	return int(mStatements.size());
}

Argument::ConstPtr Function::argument(unsigned int aIndex) const
{
	assert(aIndex < mArguments.size());
	if (aIndex >= mArguments.size()) {
		throw IndexOutOfRangeException("argument", 0, int(mArguments.size()), int(aIndex));
	}

	return mArguments[aIndex];
}

Expression::ConstRef Function::statement(unsigned int aIndex) const
{
	assert(aIndex < mStatements.size());
	if (aIndex >= mStatements.size()) {
		throw IndexOutOfRangeException("statement", 0, int(mStatements.size()), int(aIndex));
	}

	return mArguments[aIndex];
}

} // namespace Codegen
} // namespace Gfx
} // namespace Teardrop
