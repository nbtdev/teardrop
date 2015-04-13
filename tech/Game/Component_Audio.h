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
