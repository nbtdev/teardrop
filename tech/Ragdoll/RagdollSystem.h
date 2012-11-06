/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLL_SYSTEM_INCLUDED)
#define RAGDOLL_SYSTEM_INCLUDED

#include "Util/System.h"

namespace Teardrop
{
	class RagdollController;

	class RagdollSystem: public Teardrop::System
	{
	public:
		virtual RagdollController* createController() = 0;
		virtual void destroyController(RagdollController*) = 0;
	};
}

#endif // RAGDOLL_SYSTEM_INCLUDED
