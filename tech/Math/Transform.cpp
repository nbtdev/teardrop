/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#include "Transform.h"
#include "Vector4.h"
#include "Quaternion.h"

using namespace CoS;
//---------------------------------------------------------------------------
const CoS::Transform& CoS::Transform::IDENTITY = CoS::Transform(
	CoS::Vector4(0, 0, 0, 0), 
	CoS::Quaternion(1, 0, 0, 0),
	CoS::Vector4(1, 1, 1, 0)
	);
//---------------------------------------------------------------------------
Transform::~Transform()
{
}
//---------------------------------------------------------------------------
Transform::Transform(const Vector4& p, const Quaternion& r, const Vector4& s)
{
	trans = p;
	rot = r;
	scale = s;
}
//---------------------------------------------------------------------------
Transform& Transform::operator =(const Transform& other)
{
	trans = other.trans;
	rot = other.rot;
	scale = other.scale;

	return *this;
}
