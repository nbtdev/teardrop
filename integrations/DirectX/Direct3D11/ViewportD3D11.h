/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWPORTD3D11_INCLUDED)
#define VIEWPORTD3D11_INCLUDED

#include "Gfx/Viewport.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D11 
		{
			class Viewport : public Gfx::Viewport
			{
			public:
				Viewport(Gfx::RenderTarget* rt);
				~Viewport();

				D3D11_VIEWPORT& viewport();

				TD_DECLARE_ALLOCATOR();

			protected:
				D3D11_VIEWPORT mVP;
			};
		}
	}
}

#endif // VIEWPORTD3D9_INCLUDED
