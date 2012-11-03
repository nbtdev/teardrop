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

#if !defined(AUDIO_RENDERER_XAUDIO2_INCLUDED)
#define AUDIO_RENDERER_XAUDIO2_INCLUDED

#include "Renderer.h"
#include "Util/_String.h"
#include "Memory/Allocators.h"
#include <vector>
#include <map>
#include <list>

namespace CoS
{
	namespace Audio
	{
		class EmitterXAudio2;
		class ListenerXAudio2;
		class SampleXAudio2;
		class StreamXAudio2;

		/*
			XAudio2/XACT3 implementation of Audio interface
		*/
		class RendererXAudio2
			: public Renderer
		{
			IXACT3Engine* m_pEngine;

			typedef std::vector<char> Buffer;
			struct SoundBank
			{
				Buffer m_buf;
				IXACT3SoundBank* m_pSoundBank;

				// used for XACT to tell us that the sound bank is 
				// destroyed and we are free to release the underlying 
				// data (m_buf)
				HANDLE hFreeEvent;

				SoundBank() { m_pSoundBank=0; }
			};

			struct WaveBank
			{
				IXACT3WaveBank* m_pWaveBank;
				// we open large wave banks as memory-mapped files
				void* m_pMappedView;
				// smaller ones just get a memory buffer
				Buffer m_buf;

				// used for XACT to tell us that the wave bank is 
				// destroyed and we are free to release the underlying 
				// data (m_buf or m_pMappedView)
				HANDLE hFreeEvent;

				WaveBank() { m_pMappedView=0; m_pWaveBank=0; }
			};

			struct Sound
			{
				XACTINDEX m_index;
				SampleXAudio2* m_pSample;
				StreamXAudio2* m_pStream;

				Sound() { m_index=0; m_pStream=0; m_pSample=0; }
			};
			
			struct Bank
			{
				SoundBank m_soundBank;
				WaveBank m_waveBank;
				// String maps the sound to a Cue name in the SoundBank (such as "Fire")
				typedef std::map<String, Sound> Sounds;
				Sounds m_sounds;
				String m_bankPath;
			};
			// String is a "path" to the sound definition (such as "Weapon/ERLL")
			typedef std::map<String, Bank> Banks;
			Banks m_banks;

			typedef std::vector<EmitterXAudio2*> Emitters;
			typedef std::vector<ListenerXAudio2*> Listeners;
			Emitters m_emitters;
			Listeners m_listeners;

		public:
			RendererXAudio2();
			~RendererXAudio2();
			
			bool initialize();
			bool destroy();
			bool update(float deltaT);

			Handle addOrRetrieveSound(const String& filePath);
			void releaseSound(Handle hData);
			Sample* getSampleInstance(Handle hData, const String& name);
			Stream* getStreamInstance(Handle hData, const String& name);
			Emitter* createEmitter();
			void releaseEmitter(Emitter*);
			Listener* createListener();
			void releaseListener(Listener*);
			void play2D(const Stream& stream);
			void play2D(const Sample& stream);

			COS_DECLARE_ALLOCATOR();

		private:
			void releaseBank(Bank& bank);
		};
	}
}

#endif // AUDIO_RENDERER_XAUDIO2_INCLUDED
