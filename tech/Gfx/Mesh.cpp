/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Mesh.h"
#include "Submesh.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	for (size_t i=0; i<mSubmeshes.size(); ++i) {
		delete mSubmeshes[i];
	}
}

Submesh* Mesh::createSubmesh()
{
	mSubmeshes.push_back(TD_NEW Submesh);
	return mSubmeshes.back();
}

void Mesh::createSubmeshes(int nSubmeshes)
{
	assert(mSubmeshes.size()==0);

	if (mSubmeshes.size() == 0) {
		mSubmeshes.resize(nSubmeshes);

		for (int i=0; i<nSubmeshes; ++i) {
			mSubmeshes[i] = TD_NEW Submesh;
		}
	}
}

void Mesh::removeSubmesh(Submesh* submesh)
{
	for (size_t i=0; i<mSubmeshes.size(); ++i) {
		if (mSubmeshes[i] == submesh) {
			removeSubmesh(int(i));
			break;
		}
	}
}

void Mesh::removeSubmesh(int index)
{
	assert(index>=0 && index<int(mSubmeshes.size()));

	if (index >= 0 && index < int(mSubmeshes.size())) {
		delete mSubmeshes[index];
		mSubmeshes.erase(mSubmeshes.begin()+index);
	}
}

int Mesh::submeshCount()
{
	return int(mSubmeshes.size());
}

Submesh* Mesh::submesh(int index)
{
	assert(index>=0 && index<int(mSubmeshes.size()));

	if (index >= 0 && index < int(mSubmeshes.size())) {
		return mSubmeshes[index];
	}

	return 0;
}
