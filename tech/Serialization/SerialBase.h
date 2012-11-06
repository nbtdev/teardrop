/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALBASE_INCLUDED)
#define SERIALBASE_INCLUDED

namespace Teardrop
{
	class Stream;

	namespace Serial
	{
		class Base
		{
		public:
			//! called to serialize this object to a stream
			virtual bool serialize(Stream& outStream) = 0;
			//! called to deserialize this object from a stream
			virtual bool deserialize(Stream& inStream) = 0;
		};
	}
}

#endif // SERIALBASE_INCLUDED
