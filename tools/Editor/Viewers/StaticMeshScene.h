/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STATICMESHSCENE_INCLUDED)
#define STATICMESHSCENE_INCLUDED

#include "Game/Executable.h"
#include "Game/Logic.h"
#include <QWidget>

namespace Teardrop
{
	class StaticMeshAsset;

	namespace Tools
	{
		class StaticMeshScene : public Executable
		{
		public:
			TD_CLASS(StaticMeshScene, Executable);

			StaticMeshScene();
			StaticMeshScene(StaticMeshAsset* meshAsset);
			~StaticMeshScene();

			StaticMeshAsset* asset();

			TD_DECLARE_ALLOCATOR();

		private:
			StaticMeshAsset* mAsset;
		};
	}
}

#endif // STATICMESHSCENE_INCLUDED
