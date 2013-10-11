/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTDRAGDROPDATA_INCLUDED)
#define OBJECTDRAGDROPDATA_INCLUDED

#include "DragDropData.h"
#include "Util/_String.h"

// Data class for Reflection::Object items being dragged around the editor

namespace Teardrop
{
	class Package;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class ObjectDragDropData : public DragDropData
		{
		public:
			ObjectDragDropData(Reflection::Object* obj, const String& path, Package* pkg);
			~ObjectDragDropData();

			Type type();
			Reflection::Object* object();
			const String& path();
			Package* package();

		protected:
			Reflection::Object* mObject;
			String mPath; // object virtual path in its package (metadata)
			Package* mPackage; // package to which this object belongs 
		};
	}
}

#endif // OBJECTDRAGDROPDATA_INCLUDED
