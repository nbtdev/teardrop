/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UUID_INCLUDED)
#define UUID_INCLUDED

namespace Teardrop
{
	class String;

	namespace Tools
	{
		void UuidGen(/*out*/ String& uuid);
	}
}

#endif // UUID_INCLUDED
