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

#include "ExpressionConnectorDatabase.h"

namespace Teardrop {
namespace Tools {

ExpressionConnectorDatabase::ExpressionConnectorDatabase() 
{

}

ExpressionConnectorDatabase::~ExpressionConnectorDatabase() 
{

}

void 
ExpressionConnectorDatabase::addConnector(ExpressionConnector::ConstPtr aConnector, const Gfx::Attribute* aAttribute) 
{
	mDB[aAttribute] = aConnector;
}

void 
ExpressionConnectorDatabase::removeConnector(const Gfx::Attribute* aAttribute) 
{
	mDB.erase(aAttribute);
}

ExpressionConnector::ConstRef 
ExpressionConnectorDatabase::findConnectorByAttribute(const Gfx::Attribute* aAttribute) const 
{
	DB::const_iterator it = mDB.find(aAttribute);
	if (it != mDB.end())
		return it->second;

	return ExpressionConnector::ConstRef();
}

} // namespace Tools
} // namespace Teardrop
