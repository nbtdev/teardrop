/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMETADATASERIALIZER_INCLUDED)
#define PACKAGEMETADATASERIALIZER_INCLUDED

namespace Teardrop 
{
	class Package;
	class Stream;

	class PackageMetadataSerializer
	{
	public:
		virtual ~PackageMetadataSerializer();

		virtual void serialize(Package* pkg, Stream& strm) = 0;
		virtual void deserialize(Package* pkg, Stream& strm) = 0;
	};
}

#endif // PACKAGEMETADATASERIALIZER_INCLUDED
