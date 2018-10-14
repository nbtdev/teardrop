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

#if !defined(REFOBJ_INCLUDED)
#define REFOBJ_INCLUDED

#include "Gfx/Renderer.h"
#include "Math/Matrix44.h"
#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			template<typename T>
			class RefObj {
				T* mObj = nullptr;

			public:
				RefObj() : mObj(nullptr) {}
				RefObj(T* aObj) : mObj(aObj) { if (mObj) mObj->AddRef(); }
				RefObj(const RefObj& other) { *this = other; }
				~RefObj() { if (mObj) mObj->Release(); }

				RefObj& operator=(const RefObj& other) { 
					if (mObj) mObj->Release();  
					mObj = other.mObj;  
					if (mObj) mObj->AddRef(); 
				}

				T* operator->() { return mObj; }
				const T* operator->() const { return mObj; }
				operator T*() const { return mObj; }
				bool operator!() { return mObj != nullptr; }
			};
		}
	}
}

#endif // REFOBJ_INCLUDED
