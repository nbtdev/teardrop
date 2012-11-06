/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCEFACTORY_INCLUDED)
#define RESOURCEFACTORY_INCLUDED

namespace Teardrop
{
	struct FourCC;
	class Resource;
	class Allocator;

	class ResourceFactory
	{
	public:
		ResourceFactory();
		virtual ~ResourceFactory();

		virtual Resource* create(const FourCC& fourCC) = 0;
		virtual Resource* createPlaceholder(const FourCC& fourCC) = 0;
		virtual void destroy(Resource*, const FourCC& fourCC) = 0;
		virtual Allocator* getAllocator() = 0;
	};
}

#endif // RESOURCEFACTORY_INCLUDED
