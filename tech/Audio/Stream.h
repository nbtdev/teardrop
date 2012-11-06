/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_STREAM_INCLUDED)
#define AUDIO_STREAM_INCLUDED

namespace Teardrop
{
	namespace Audio
	{
		/*
			Stream class encapsulates behavior of streaming audio
		*/
		class Stream
		{
		public:
			Stream();
			virtual ~Stream();
		};
	}
}

#endif // AUDIO_STREAM_INCLUDED
