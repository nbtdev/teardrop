/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
