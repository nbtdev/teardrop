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

#if !defined(LOGIC_INCLUDED)
#define LOGIC_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	class Executable;

	class Logic : public Reflection::Object
	{
	public:
		TD_CLASS(Logic, Object);
		TD_POINTER_PROPERTY(CameraController, "", CameraController, 0);

		Logic();
		~Logic();

		virtual bool attached(Executable* exe);
		virtual bool onPreLoad(Executable* exe);
		virtual bool onPostLoad(Executable* exe);
		virtual bool update(Executable* exe);
		virtual bool onPreUnload(Executable* exe);
		virtual bool onPostUnload(Executable* exe);
		virtual bool detached(Executable* exe);

		virtual void injectMouseMove(int absX, int absY, int relX=0, int relY=0);
		virtual void injectMouseWheel(int absZ, int relZ=0);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // LOGIC_INCLUDED
