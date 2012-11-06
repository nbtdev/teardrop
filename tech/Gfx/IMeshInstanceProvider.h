/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MESHINSTANCEPROVIDER_INCLUDED)
#define MESHINSTANCEPROVIDER_INCLUDED

namespace Teardrop
{
	class GfxRenderer;

	class IMeshInstanceProvider
	{
	public:
		virtual void queueForRendering(GfxRenderer* pRend) = 0;
	};
}

#endif // MESHINSTANCEPROVIDER_INCLUDED
