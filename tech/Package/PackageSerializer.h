/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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

		// 12 bytes reserved for future use
		int mReserved[3];
	};

	class PackageMetadataSerializer;

	class PackageSerializer
	{
	public:
		PackageSerializer(Package* pkg);
		~PackageSerializer();

		static const int PACKAGE_VERSION;

        uint64_t serialize(Stream& stream, PackageMetadataSerializer* metadataSerializer = 0);
        uint64_t deserialize(Stream& stream, DeferredObjectResolves& deferred, ObjectIdToObject& lut, PackageMetadataSerializer* metadataSerializer = 0);

	protected:
		Package* mPkg;
	};
}

#endif // PACKAGESERIALIZER_INCLUDED
