/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ADDCOLOREXPRESSION_INCLUDED)
#define ADDCOLOREXPRESSION_INCLUDED

#include "Gfx/MaterialExpression.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class AddColorExpression : public MaterialExpression
		{
		public:
			TD_CLASS(AddColorExpression, MaterialExpression);
			TD_CLASS_CREATABLE();

			AddColorExpression();
			~AddColorExpression();

			bool initialize();

			TD_DECLARE_ALLOCATOR();

		protected:
			// code generation
			void appendBody(Language lang, std::ostream& o);
		};
	}
}

#endif // ADDCOLOREXPRESSION_INCLUDED
