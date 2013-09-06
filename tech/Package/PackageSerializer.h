/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGESERIALIZER_INCLUDED)
#define PACKAGESERIALIZER_INCLUDED

#include "Package/DeferredResolution.h"

namespace Teardrop 
{
	class Stream;
	class Package;

	struct PackageHeader
	{
		// package format version
		int mVersion;

		// number of sections in the package
		int mNumSections;

		// 8 bytes reserved for future use
		int mReserved[2];
	};

	class PackageMetadataSerializer;

	class PackageSerializer
	{
	public:
		PackageSerializer(Package* pkg);
		~PackageSerializer();

		static const int PACKAGE_VERSION;

		bool serialize(Stream& stream, PackageMetadataSerializer* metadataSerializer = 0);
		bool deserialize(Stream& stream, DeferredObjectResolves& deferred, ObjectIdToObject& lut, PackageMetadataSerializer* metadataSerializer = 0);

	protected:
		Package* mPkg;
	};
}

#endif // PACKAGESERIALIZER_INCLUDED
