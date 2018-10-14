/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "Metadata.h"
#include "Reflection/PropertyDef.h"
#include "Util/_String.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(Metadata);

Metadata::Metadata()
	: mObject(0)
{

}

Metadata::~Metadata()
{
}

void Metadata::notifyPropertyChangedLocal(const Reflection::PropertyDef* prop)
{
	if (String("Name") == prop->getName()) {
        NameChanged.raise((const char*)getName());
	}
}

void Metadata::generateThumbnail()
{
}

const Thumbnail& Metadata::thumbnail()
{
	return mThumb;
}

Reflection::Object* Metadata::object()
{
	return mObject;
}
