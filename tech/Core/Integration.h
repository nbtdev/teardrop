/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INTEGRATION_INCLUDED)
#define INTEGRATION_INCLUDED

namespace Teardrop
{
	class Integration
	{
	public:
		Integration();
		virtual ~Integration();
		virtual void load() = 0;
		virtual void unload() = 0;
	};
}

#endif // INTEGRATION_INCLUDED
