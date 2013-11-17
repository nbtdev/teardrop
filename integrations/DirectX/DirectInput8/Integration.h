/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef DIRECTINPUT8_INTEGRATION_H
#define DIRECTINPUT8_INTEGRATION_H

#include "Core/Integration.h"

namespace Teardrop 
{
	namespace DirectInput {
		
		class Input;

		class Integration : public Teardrop::Integration
		{
		public:
			Integration();
			~Integration();
			void load();
			void unload();

		private:
			Input* mInput;
		};
	}
}

#endif // DIRECTINPUT8_INTEGRATION_H
