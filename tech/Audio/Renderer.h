/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_RENDERER_INCLUDED)
#define AUDIO_RENDERER_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	class String;

	namespace Audio
	{
		class Emitter;
		class Listener;
		class Stream;
		class Sample;
		typedef void* Handle;

		/*
			Interface to audio rendering engine
		*/
		class Renderer
		{
		public:
			Renderer();
			virtual ~Renderer();

			virtual bool initialize() = 0;
			virtual bool destroy() = 0;
			virtual bool update(float deltaT) = 0;

			//! 'path' is relative to the root data path, without any file or object name at the end
			virtual Handle addOrRetrieveSound(const String& filePath) = 0;
			virtual void releaseSound(Handle hData) = 0;
			//! hData was obtained via addOrRetrieveSound, and 'name' is the sample name
			virtual Sample* getSampleInstance(Handle hData, const String& name) = 0;
			//! hData was obtained via addOrRetrieveSound, and 'name' is the stream name
			virtual Stream* getStreamInstance(Handle hData, const String& name) = 0;
			virtual Emitter* createEmitter() = 0;
			virtual void releaseEmitter(Emitter*) = 0;
			virtual Listener* createListener() = 0;
			virtual void releaseListener(Listener*) = 0;
			virtual void play2D(const Stream& stream) = 0;
			virtual void play2D(const Sample& stream) = 0;

			static Renderer* createInstance();
			static void destroyInstance(Renderer*);
		};
	}
}

#endif // AUDIO_RENDERER_INCLUDED
