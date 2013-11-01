/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTVIEWERLOGIC_INCLUDED)
#define OBJECTVIEWERLOGIC_INCLUDED

#include "Game/Logic.h"

namespace Teardrop
{
	namespace Tools
	{
		/**
			ObjectViewerLogic provides a single orbit camera focused on a single object
			in the scene to which it is attached
		**/
		class ObjectViewerLogic : public Logic
		{
		public:
			TD_CLASS(ObjectViewerLogic, Logic);

			ObjectViewerLogic();
			~ObjectViewerLogic();

			bool onPreLoad(Executable* exe);
			bool onPostLoad(Executable* exe);
			bool update(Executable* exe);
			bool onPreUnload(Executable* exe);
			bool onPostUnload(Executable* exe);

			TD_DECLARE_ALLOCATOR();

		private:
		};
	}
}

#endif // OBJECTVIEWERLOGIC_INCLUDED
