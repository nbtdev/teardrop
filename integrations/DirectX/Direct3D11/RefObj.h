/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
