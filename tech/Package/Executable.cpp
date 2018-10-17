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

#include "Executable.h"
#include "Logic.h"

using namespace Teardrop;

TD_CLASS_IMPL(Executable);

Executable::Executable()
{

}

Executable::~Executable()
{

}

bool Executable::onPreLoad()
{
	if (getLogic())
		return getLogic()->onPreLoad(this);

	return true;
}

bool Executable::onPostLoad()
{
	if (getLogic())
		return getLogic()->onPostLoad(this);

	return true;
}

bool Executable::onPreUnload()
{
	if (getLogic())
		return getLogic()->onPreUnload(this);

	return true;
}

bool Executable::onPostUnload()
{
	if (getLogic())
		return getLogic()->onPostUnload(this);

	return true;
}

void Executable::tick()
{
	if (getLogic())
		getLogic()->update(this);
}

void Executable::renderFrame(Gfx::Renderer* /*renderer*/, Gfx::RenderTarget* /*rt*/)
{
}

void Executable::notifyPropertyChangingLocal(const Reflection::PropertyDef* pPropDef)
{
	if (pPropDef == getLogicDef()) {
		Logic* logic = getLogic();
		if (logic) {
			logic->detached(this);
		}
	}
}

void Executable::notifyPropertyChangedLocal(const Reflection::PropertyDef* pPropDef)
{
	if (pPropDef == getLogicDef()) {
		Logic* logic = getLogic();
		if (logic) {
			logic->attached(this);
		}
	}
}

void Executable::injectMouseMove(int absX, int absY, int relX, int relY)
{
}

void Executable::injectMouseWheel(int absZ, int relZ)
{
}
