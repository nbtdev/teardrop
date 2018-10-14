/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "ShaderFeatures.h"

using namespace Teardrop;
using namespace Gfx;

ShaderFeatures::ShaderFeatures()
	: mInterpolants(0)
	, mTexcoordMask(0)
{

}

void ShaderFeatures::setFeature(Interpolants interp, int texCoord/* =-1 */)
{
	mInterpolants |= interp;
	if (texCoord >= 0 && texCoord < 16) {
		mTexcoordMask |= (unsigned short)(1 << texCoord);
	}
}

void ShaderFeatures::merge(const ShaderFeatures& other)
{
	mInterpolants |= other.mInterpolants;
	mTexcoordMask |= other.mTexcoordMask;
}

void ShaderFeatures::clear()
{
	mInterpolants = 0;
	mTexcoordMask = 0;
}

bool ShaderFeatures::isEquivalent(const ShaderFeatures& other) const
{
	return (mInterpolants==other.mInterpolants) && (mTexcoordMask==other.mTexcoordMask);
}

bool ShaderFeatures::hasAtLeast(const ShaderFeatures& other) const
{
	// a bit more tricky...we have to ensure that we have at least the features that the 
	// "other" possesses; we can do that by and'ing the masks and comparing to the "other"
	// (if the we have at least everything "they" have then the two will match, even if
	// we have more). Then if there are texcoords present, do the same with the texcoord
	// mask
	unsigned short tmp = mInterpolants & other.mInterpolants;
	if (tmp == other.mInterpolants) {
		tmp = mTexcoordMask & other.mTexcoordMask;
		return (tmp == other.mTexcoordMask);
	}

	return false;
}

bool ShaderFeatures::isEmpty() const
{
	return (mInterpolants==0);
}
