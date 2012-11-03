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
#include "SampleXAudio2.h"
#include "Math/MathUtil.h"

using namespace CoS;
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
