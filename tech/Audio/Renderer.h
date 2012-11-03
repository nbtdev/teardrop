/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(AUDIO_RENDERER_INCLUDED)
#define AUDIO_RENDERER_INCLUDED

#include "Memory/Memory.h"

namespace CoS
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
