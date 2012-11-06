/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDTRANSFORM_INCLUDED)
#define TDTRANSFORM_INCLUDED

#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace Teardrop
{
	class Transform
	{
	public:
		Vector4 trans;
		Quaternion rot;
		Vector4 scale;

		Transform();
		Transform(const Transform& other);
		Transform(const Vector4& pos, const Quaternion& rot, const Vector4& scale);
		~Transform();
		Transform& operator=(const Transform& other);

		static const Transform& IDENTITY;
	};

	inline Transform::Transform()
	{
	}

	inline Transform::Transform(const Transform& other)
	{
		*this = other;
	}
}

#endif // TDTRANSFORM_INCLUDED
