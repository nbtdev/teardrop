/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SAMPLER2DEXPRESSION_INCLUDED)
#define SAMPLER2DEXPRESSION_INCLUDED

#include "Gfx/MaterialExpression.h"
#include "Gfx/Sampler2D.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Sampler2DExpression : public MaterialExpression
		{
		public:
			TD_CLASS(Sampler2DExpression, MaterialExpression);
			TD_CLASS_CREATABLE();
			TD_NESTED_PROPERTY(Sampler2D, "2D Texture Sampler", Sampler2D);

			Sampler2DExpression();
			~Sampler2DExpression();

			bool initialize();

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // SAMPLER2DEXPRESSION_INCLUDED
