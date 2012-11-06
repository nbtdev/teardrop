/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
