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

#if !defined(AUDIO_SAMPLE_INCLUDED)
#define AUDIO_SAMPLE_INCLUDED

namespace Teardrop
{
	namespace Audio
	{
		class Emitter;
		class Listener;

		/*
			Sample class encapsulates behavior of audio samples
		*/
		class Sample
		{
		public:
			Sample();
			virtual ~Sample();

			//! play sample to end; will also initialize the sample length
			virtual void play2D() = 0;
			//! play sample to end in 3D space; will also initialize the sample length
			virtual void play3D(Emitter* pEmitter, Listener* pListener) = 0;
			//! returns sample length in seconds
			virtual float getLength() = 0;
		};
	}
}

#endif // AUDIO_SAMPLE_INCLUDED
