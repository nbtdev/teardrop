/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DEFERREDRESOLUTION_INCLUDED)
#define DEFERREDRESOLUTION_INCLUDED

#include <list>
#include <map>
#include "Util/UUID.h"

namespace Teardrop 
{
	namespace Reflection
	{
		class Object;
		class PropertyDef;
	}

	struct DeferredResolution
	{
		Reflection::Object* mObject;
		const Reflection::PropertyDef* mProp;
		UUID mUUID;
	};

	typedef std::list<DeferredResolution> DeferredObjectResolves;
	typedef std::map<UUID, Reflection::Object*> ObjectIdToObject;
}

#endif // DEFERREDRESOLUTION_INCLUDED

