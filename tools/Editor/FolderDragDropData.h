/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FOLDERDRAGDROPDATA_INCLUDED)
#define FOLDERDRAGDROPDATA_INCLUDED

#include "DragDropData.h"
#include "Util/_String.h"

// Data class for Reflection::Object items being dragged around the editor

namespace Teardrop
{
	class Package;

	namespace Tools
	{
		class Folder;
		class FolderDragDropData : public DragDropData
		{
		public:
			FolderDragDropData(Folder* folder, const String& path, Package* pkg);
			~FolderDragDropData();

			Type type();
			Folder* folder();
			const String& path();
			Package* package();

		protected:
			Folder* mFolder;
			String mPath; // folder virtual path in its package (metadata)
			Package* mPackage; // package to which this folder belongs 
		};
	}
}

#endif // FOLDERDRAGDROPDATA_INCLUDED
