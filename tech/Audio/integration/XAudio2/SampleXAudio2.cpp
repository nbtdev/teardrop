/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "SampleXAudio2.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
using namespace Audio;
//---------------------------------------------------------------------------
SampleXAudio2::SampleXAudio2(
	IXACT3SoundBank* pBank, XACTINDEX index)
: m_index(index)
, m_pSoundBank(pBank)
, m_length(-1)
{
	IXACT3Cue* pCue;

	if (m_pSoundBank)
		m_pSoundBank->Prepare(m_index, 0, 0, &pCue);

	if (pCue && m_length < 0)
	{
		// initialize the cue length
		XACT_CUE_INSTANCE_PROPERTIES* props;
		pCue->GetProperties(&props);

		// TODO: do we need to support more than one track per cue?
		m_length = 
			float(props->activeVariationProperties.soundProperties.arrTrackProperties[0].duration) / 1000.f;

		CoTaskMemFree((void*)props);
	}
}
//---------------------------------------------------------------------------
SampleXAudio2::~SampleXAudio2()
{
}
//---------------------------------------------------------------------------
void SampleXAudio2::play2D()
{
	if (m_pSoundBank)
		m_pSoundBank->Play(m_index, 0, 0, 0);
}
//---------------------------------------------------------------------------
void SampleXAudio2::play3D(Emitter* pEmitter, Listener* pListener)
{
}
//---------------------------------------------------------------------------
float SampleXAudio2::getLength()
{
	return m_length;
}
