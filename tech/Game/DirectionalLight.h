/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DIRECTIONALLIGHT_INCLUDED)
#define DIRECTIONALLIGHT_INCLUDED

#include "Game/DynamicLight.h"

namespace Teardrop
{
	class DirectionalLight
		: public DynamicLight
	{

	public:
		TD_CLASS(DirectionalLight, DynamicLight);
		TD_CLASS_CREATABLE();

		DirectionalLight();
		virtual ~DirectionalLight();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // DIRECTIONALLIGHT_INCLUDED
