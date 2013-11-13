/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWPORTD3D9_INCLUDED)
#define VIEWPORTD3D9_INCLUDED

#include "Gfx/Viewport.h"

namespace Teardrop
{
	namespace Gfx {
		namespace Direct3D9 {
			class Viewport : public Gfx::Viewport
			{
			public:
				Viewport(Gfx::RenderTarget* rt);
				~Viewport();

				D3DVIEWPORT9& viewport();

				TD_DECLARE_ALLOCATOR();

			protected:
				D3DVIEWPORT9 mVP;
			};
		}
	}
}

#endif // VIEWPORTD3D9_INCLUDED
