/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIALOUTPUT_INCLUDED)
#define MATERIALOUTPUT_INCLUDED

#include "Gfx/MaterialExpression.h"

namespace Teardrop
{
	namespace Gfx 
	{
		/*
			MaterialOutput is handled specially, and serves as the final output 
			ports for the rest of the material expression graph. It is created 
			when a Material is created in the editor
		*/

		class MaterialOutput : public MaterialExpression
		{
		public:
			TD_CLASS(MaterialOutput, MaterialExpression);

			MaterialOutput();
			~MaterialOutput();

			bool initialize();

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // MATERIALOUTPUT_INCLUDED
