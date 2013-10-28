/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STATICMESHASSET_INCLUDED)
#define STATICMESHASSET_INCLUDED

#include "Asset/Asset.h"

namespace Teardrop
{
	namespace Gfx {
		class Material;
		class Mesh;
	}

	class StaticMeshAsset : public Asset
	{
	public:
		TD_CLASS(StaticMeshAsset, Asset);
		TD_CLASS_CREATABLE();
		TD_POINTER_PROPERTY(Material, "Default material for this asset", Gfx::Material, 0);

		StaticMeshAsset();
		~StaticMeshAsset();

		// will create if not already present
		Gfx::Mesh* mesh();

		int serialize(Stream& strm);
		int deserialize(Stream& strm);

		TD_DECLARE_ALLOCATOR();

	protected:
		Gfx::Mesh* mMesh;
	};
} // namespace Teardrop

#endif // STATICMESHASSET_INCLUDED
