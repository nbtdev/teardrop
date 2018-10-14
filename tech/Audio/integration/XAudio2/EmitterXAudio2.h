/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
