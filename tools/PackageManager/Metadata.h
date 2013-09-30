/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(METADATA_INCLUDED)
#define METADATA_INCLUDED

#include "PackageManager/Thumbnail.h"
#include "Reflection/Reflection.h"
#include "Memory/Memory.h"
#include "FastDelegate.h"

namespace Teardrop
{
	namespace Tools
	{
		class Thumbnail;

		class Metadata : public Reflection::Object
		{
		public:
			TD_CLASS(Metadata, Object);
			TD_PROPERTY(Name, "Object Name", String, "(undefined)", 0);
			TD_PROPERTY(ID, "Object ID", String, "(undefined)", "ReadOnly");

			Metadata();
			~Metadata();

			// default implementation just uses the default Teardrop icon
			virtual void generateThumbnail();
			const Thumbnail& thumbnail();

			fastdelegate::FastDelegate1<const char*> NameChanged;

			Reflection::Object* object();

			TD_DECLARE_ALLOCATOR();

		protected:
			void notifyPropertyChangedLocal(const Reflection::PropertyDef* prop);
			Thumbnail mThumb;

			friend class PackageMetadata;
			Reflection::Object* mObject;
		};
	}
}

#endif // METADATA_INCLUDED
