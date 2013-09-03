/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTMETADATA_INCLUDED)
#define OBJECTMETADATA_INCLUDED

#include "PackageManager/Metadata.h"
#include <map>

namespace Teardrop 
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class ObjectMetadata : public Metadata
		{
		public:
			TD_CLASS(ObjectMetadata, Metadata);

			ObjectMetadata();
			ObjectMetadata(Reflection::Object* obj);
			~ObjectMetadata();

			TD_DECLARE_ALLOCATOR();

		protected:
			Reflection::Object* mObject;

			void onPropertyChanged(const Reflection::PropertyDef* prop);
		};
	}
}

#endif // OBJECTMETADATA_INCLUDED
