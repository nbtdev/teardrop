/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCE_INCLUDED)
#define RESOURCE_INCLUDED

namespace Teardrop
{
	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		// destroy dynamically-created resources
		virtual bool destroy() = 0;
		// allow non-dynamic resources to clean up any dynamic data
		virtual bool release() = 0;
	};
}

#endif // RESOURCE_INCLUDED
