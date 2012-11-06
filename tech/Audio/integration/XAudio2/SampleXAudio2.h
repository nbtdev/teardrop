/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_SAMPLE_XAUDIO2_INCLUDED)
#define AUDIO_SAMPLE_XAUDIO2_INCLUDED

#include "Sample.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Audio
	{
		/*
			XAudio2 implementation of Sample
		*/
		class SampleXAudio2
			: public Sample
		{
			XACTINDEX m_index;
			IXACT3SoundBank* m_pSoundBank;
			float m_length;

		public:
			SampleXAudio2(IXACT3SoundBank* pBank, XACTINDEX index);
			~SampleXAudio2();

			void play2D();
			void play3D(Emitter* pEmitter, Listener* pListener);
			float getLength();

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // AUDIO_SAMPLE_XAUDIO2_INCLUDED
