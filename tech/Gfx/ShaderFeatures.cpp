/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
