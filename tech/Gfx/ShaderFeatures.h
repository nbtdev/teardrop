/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADERFEATURES_INCLUDED)
#define SHADERFEATURES_INCLUDED

namespace Teardrop
{
	namespace Gfx 
	{
		enum Interpolants {
			// Note: position must be present in vertex shader output; all others are optional
			INTERP_POSITION = 1,
			INTERP_NORMAL = 2,
			INTERP_TEXCOORD = 4,
			INTERP_TANGENT = 4,
			INTERP_BITANGENT = 4,
		};

		struct ShaderFeatures
		{
			ShaderFeatures();

			unsigned short mInterpolants;
			unsigned short mTexcoordMask;

			void setFeature(Interpolants interp, int texCoord=-1);
			void merge(const ShaderFeatures& other);
			void clear();

			bool isEquivalent(const ShaderFeatures& other) const;
			bool hasAtLeast(const ShaderFeatures& other) const;
			bool isEmpty() const;
		};

		inline bool operator<(const ShaderFeatures& l, const ShaderFeatures& r) {
			return (((unsigned long long)(l.mInterpolants)) << 32 | (unsigned long long) l.mTexcoordMask) < 
				(((unsigned long long)(r.mInterpolants)) << 32 | (unsigned long long) r.mTexcoordMask);
		}
	}
}

#endif // SHADERFEATURES_INCLUDED
