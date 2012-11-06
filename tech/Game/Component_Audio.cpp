/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_Audio.h"
#include "Audio/Sample.h"
#include "Game/Manifest.h"
#include "Util/Environment.h"

using namespace Teardrop;
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
			m_sfx[idx].pDelayTrigger->Fire.bind(this, &AudioComponent::play);
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
