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

#if !defined(EXECUTABLE_INCLUDED)
#define EXECUTABLE_INCLUDED

#include "Core/Logic.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	namespace Gfx {
		class Renderer;
		class RenderTarget;
	}

	class Executable : public Reflection::Object
	{
	public:
		TD_CLASS(Executable, Object);
		TD_POINTER_PROPERTY(Logic, "Pluggable logic module for this executable", Logic, 0);

		Executable();
		~Executable();

		virtual bool onPreLoad();
		virtual bool onPostLoad();
		virtual void tick();
		virtual void renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt);
		virtual bool onPreUnload();
		virtual bool onPostUnload();

        // input injection
        virtual void injectMouseMove(int absX, int absY, int relX = 0, int relY = 0);
        virtual void injectMouseWheel(int absZ, int relZ = 0);

		TD_DECLARE_ALLOCATOR();

	protected:
		void notifyPropertyChangedLocal(const Reflection::PropertyDef* pPropDef);
		void notifyPropertyChangingLocal(const Reflection::PropertyDef* pPropDef);

	private:
	};
}

#endif // EXECUTABLE_INCLUDED
