/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERCONTROLLER_INCLUDED)
#define PLAYERCONTROLLER_INCLUDED

namespace Teardrop
{
	// abstract interface defining the entry points for player controller implementations
	class PlayerController
	{
	public:
		virtual ~PlayerController();

		virtual void update(float deltaT) = 0;
	};
}

#endif // PLAYERCONTROLLER_INCLUDED
