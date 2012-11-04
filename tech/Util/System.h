/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SYSTEM_H_INCLUDED)
#define SYSTEM_H_INCLUDED

namespace Teardrop
{
	class System
	{
	public:

		// types of pluggable systems that the runtime knows about
		enum Type
		{
			SYSTEM_ANIMATION,
			SYSTEM_AUDIO,
			SYSTEM_GRAPHICS,
			SYSTEM_PHYSICS,
			SYSTEM_NETWORK,
			SYSTEM_RAGDOLL,
			SYSTEM_SCRIPT,
			SYSTEM_TASK_SCHEDULER,
			SYSTEM_UI,
			SYSTEM_VFX,

			SYSTEM_TYPE_MAX
		};

		virtual void initialize() = 0;
		virtual void shutdown() = 0;
		/** retrieve the system types this implementation supports; pass in the nunmber of elements
		    in the Type array in typeCount, returns number of elements in the populated array
		**/
		virtual void getTypes(/*out*/ Type* typeArray, /*inout*/ int& typeCount) = 0;
	};
}

#endif // SYSTEM_H_INCLUDED