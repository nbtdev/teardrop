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
