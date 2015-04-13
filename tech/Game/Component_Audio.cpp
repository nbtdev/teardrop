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

#include "Component_Audio.h"
#include "Audio/Sample.h"
#include "Game/Manifest.h"
#include "Util/Environment.h"

using namespace Teardrop;
using namespace std::placeholders;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(AudioComponent);
//---------------------------------------------------------------------------
AudioComponent::AudioComponent()
{
}
//---------------------------------------------------------------------------
AudioComponent::~AudioComponent()
{
}
//---------------------------------------------------------------------------
void AudioComponent::onInstanceCreated()
{
	setServerComponent(false); // audio not needed on a server
}
//---------------------------------------------------------------------------
bool AudioComponent::initialize()
{
	// initialize our SFX too
	m_sfx.resize(getSounds().getCount());

	Audio::Renderer* pAudio = Environment::get().pAudio;

	for (size_t i=0; i<m_sfx.size(); ++i)
	{
		m_hAudio = pAudio->addOrRetrieveSound(m_assetPath);

		String soundName;
		getSounds().getAt(i, soundName);

		m_sfx[i].pSample = pAudio->getSampleInstance(m_hAudio, soundName);
	}

	return true;
}
//---------------------------------------------------------------------------
bool AudioComponent::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool AudioComponent::update(float deltaT)
{
	// update our triggers, if any
	// bear in mind, this may cause callbacks to fire
	for (size_t i=0; i<m_sfx.size(); ++i)
	{
		if (m_sfx[i].pDelayTrigger)
		{
			m_sfx[i].pDelayTrigger->update(deltaT);
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void AudioComponent::play2D(size_t idx, float delay, bool finishPlayingBy)
{
	if (idx >= m_sfx.size())
		return;

	// start the recycle countdown timer (create the trigger if doesn't exist)
	if (delay > 0)
	{
		if (!m_sfx[idx].pDelayTrigger)
		{
			float delayTime = delay;
			if (finishPlayingBy)
				delayTime -= m_sfx[idx].pSample->getLength();

			m_sfx[idx].pDelayTrigger = TD_NEW DelayedAudioTrigger(
				m_sfx[idx].pSample, true, delayTime);


			// connect our play callback
            m_sfx[idx].pDelayTrigger->Fire.bind(std::bind(&AudioComponent::play, this, _1));
		}

		m_sfx[idx].pDelayTrigger->setEnabled(true);
		m_sfx[idx].pDelayTrigger->reset();
	}
	else
	{
		play2D(m_sfx[idx].pSample);
	}
}
//---------------------------------------------------------------------------
void AudioComponent::play(Trigger* pTrigger)
{
	DelayedAudioTrigger* pSound = static_cast<DelayedAudioTrigger*>(pTrigger);
	if (pSound->m_b2D)
		play2D(pSound->m_pSample);
	else
		play3D(pSound->m_pSample);
}
//---------------------------------------------------------------------------
void AudioComponent::play2D(Audio::Sample* pSample)
{
	if (pSample)
		pSample->play2D();
}
//---------------------------------------------------------------------------
void AudioComponent::play3D(Audio::Sample* /*pSample*/)
{
}
