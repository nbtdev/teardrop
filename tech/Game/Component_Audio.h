/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_AUDIO_INCLUDED)
#define COMPONENT_AUDIO_INCLUDED

#include "Game/Component.h"
#include "Game/CountdownTrigger.h"
#include "Audio/Renderer.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class AudioComponent
		: public Component
	{
	public:
		TD_CLASS(AudioComponent, Component);
		TD_COLLECTION(Sounds, "Sounds", String);

		AudioComponent();
		~AudioComponent();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		void play2D(size_t idx, float delay=-1, bool finishPlayingBy=false);
		void play3D(size_t idx, float delay=-1, bool finishPlayingBy=false);

		TD_DECLARE_ALLOCATOR();

	protected:
		struct DelayedAudioTrigger 
			: public CountdownTrigger
		{
			DelayedAudioTrigger(
				Audio::Sample* pSample, 
				bool is2D,
				float startPlayingAfter, 
				bool repeat=false) 
				: CountdownTrigger(startPlayingAfter, repeat)
			    , m_b2D(is2D), m_pSample(pSample) {}

			Audio::Sample* m_pSample;
			bool m_b2D;
		};

		Audio::Handle m_hAudio;
		Audio::Emitter* m_pEmitter;

		struct Sound
		{
			Audio::Sample* pSample;
			DelayedAudioTrigger* pDelayTrigger;
			Sound() { pSample=0; pDelayTrigger=0; }
		};
		typedef std::vector<Sound> SoundEffects;
		SoundEffects m_sfx;

		void play(Trigger* pTrigger);
		void play2D(Audio::Sample* pSample);
		void play3D(Audio::Sample* pSample);

		void onInstanceCreated();
	};
}

#endif // COMPONENT_AUDIO_INCLUDED
