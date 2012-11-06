/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_STREAM_XAUDIO2_INCLUDED)
#define AUDIO_STREAM_XAUDIO2_INCLUDED

#include "Stream.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Audio
	{
		/*
			XAudio2 implementation of Stream
		*/
		class StreamXAudio2
			: public Stream
		{
			XACTINDEX m_index;

		public:
			StreamXAudio2(XACTINDEX index);
			~StreamXAudio2();

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // AUDIO_STREAM_XAUDIO2_INCLUDED
