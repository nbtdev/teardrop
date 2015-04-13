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


#include "AddColorExpression.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(AddColorExpression);

AddColorExpression::AddColorExpression()
{
}

AddColorExpression::~AddColorExpression()
{
}

bool AddColorExpression::initialize()
{
	mInputs.push_back(Attribute("A", ATTR_RGBA, this));
	mInputs.push_back(Attribute("B", ATTR_RGBA, this));

	mOutputs.push_back(Attribute("Output", ATTR_RGBA, this));

	return true;
}

static const char* BODY =
"    c = a + b;\n"
;

void AddColorExpression::appendBody(Language /*lang*/, std::ostream& o)
{
	// it's the same in all languages
    o << std::string(BODY);
}
