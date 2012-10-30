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

#include "stdafx.h"
#include "EmitterXAudio2.h"
#include "Math/include/Transform.h"

using namespace CoS;
using namespace Audio;
//---------------------------------------------------------------------------
EmitterXAudio2::EmitterXAudio2()
{
	memset(&m_x3dEmitter, 0, sizeof(m_x3dEmitter));
	m_x3dEmitter.ChannelCount = 1;
	m_x3dEmitter.CurveDistanceScaler = 1;
	m_x3dEmitter.DopplerScaler = 1;
}
//---------------------------------------------------------------------------
EmitterXAudio2::~EmitterXAudio2()
{
}
//---------------------------------------------------------------------------
void EmitterXAudio2::initialize()
{
}
//---------------------------------------------------------------------------
void EmitterXAudio2::destroy()
{
}
//---------------------------------------------------------------------------
void EmitterXAudio2::release()
{
}
//---------------------------------------------------------------------------
void EmitterXAudio2::setTransformWS(const Transform& transform)
{
	m_x3dEmitter.Position.x = transform.trans.x;
	m_x3dEmitter.Position.y = transform.trans.y;
	m_x3dEmitter.Position.z = transform.trans.z;

	Vector4 orientFront(transform.rot * Vector4::UNIT_Z);
	m_x3dEmitter.OrientFront.x = orientFront.x;
	m_x3dEmitter.OrientFront.y = orientFront.y;
	m_x3dEmitter.OrientFront.z = orientFront.z;

	Vector4 orientTop(transform.rot * Vector4::UNIT_Y);
	m_x3dEmitter.OrientTop.x = orientTop.x;
	m_x3dEmitter.OrientTop.y = orientTop.y;
	m_x3dEmitter.OrientTop.z = orientTop.z;
}
