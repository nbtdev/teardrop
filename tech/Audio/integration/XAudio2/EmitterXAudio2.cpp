/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "EmitterXAudio2.h"
#include "Math/Transform.h"

using namespace Teardrop;
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
