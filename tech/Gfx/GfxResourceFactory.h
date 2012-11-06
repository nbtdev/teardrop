/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRESOURCEFACTORY_INCLUDED)
#define GFXRESOURCEFACTORY_INCLUDED

#include "Resource/ResourceFactory.h"

namespace Teardrop
{
	struct Environment;

	class GfxResourceFactory : public ResourceFactory
	{
		Environment& m_env;

	public:
		GfxResourceFactory(Environment& env);
		~GfxResourceFactory();

		Resource* create(const FourCC& fourCC);
		Resource* createPlaceholder(const FourCC& fourCC);
		void destroy(Resource*, const FourCC& fourCC);
		Allocator* getAllocator();

		DECLARE_GFX_ALLOCATOR();

	private:
		GfxResourceFactory(GfxResourceFactory& other);
		GfxResourceFactory& operator=(GfxResourceFactory& other);
	};
}

#endif // GFXRESOURCEFACTORY_INCLUDED
