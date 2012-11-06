/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_EMITTER_XAUDIO2_INCLUDED)
#define AUDIO_EMITTER_XAUDIO2_INCLUDED

#include "Emitter.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Audio
	{
		/*
			XAudio2 implementation of Emitter 
		*/
		class EmitterXAudio2
			: public Emitter
		{
			X3DAUDIO_EMITTER m_x3dEmitter;

		public:
			EmitterXAudio2();
			~EmitterXAudio2();

			//! 'source' is reference to Stream or Sample
			void initialize();
			void destroy();
			void setTransformWS(const Transform& transform);

			// called by RendererXAudio2 when the underlying audio data is going away
			void release();
			// get rad/write reference to the contained X3DAUDIO_EMITTER structure 
			X3DAUDIO_EMITTER& getX3DAudioEmitter() { return m_x3dEmitter; }

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // AUDIO_EMITTER_XAUDIO2_INCLUDED
