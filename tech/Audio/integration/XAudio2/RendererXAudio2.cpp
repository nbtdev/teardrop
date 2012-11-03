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

#include "stdafx.h"
#include "RendererXAudio2.h"
#include "SampleXAudio2.h"
#include "StreamXAudio2.h"
#include "EmitterXAudio2.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace CoS;
using namespace Audio;
//---------------------------------------------------------------------------
RendererXAudio2::RendererXAudio2()
{
	m_pEngine = 0;
}
//---------------------------------------------------------------------------
RendererXAudio2::~RendererXAudio2()
{
}
//---------------------------------------------------------------------------
void __stdcall SoundOrWaveBankDestroyedNotificationCallback(
	const XACT_NOTIFICATION *pNotification )
{
	if( ( NULL != pNotification ) && ( NULL != pNotification->pvContext ) )
	{
		SetEvent( (HANDLE) pNotification->pvContext );
	}
}
//---------------------------------------------------------------------------
bool RendererXAudio2::initialize()
{
	XACT3CreateEngine(0, &m_pEngine);
	if (!m_pEngine)
	{
		Environment::get().pLogger->logMessage(
			"*** Could not create audio renderer instance ***");
		return false;
	}

	HRESULT hr;

	// Initialize & create the XACT runtime 
	XACT_RUNTIME_PARAMETERS xrParams = {0};
	xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	xrParams.fnNotificationCallback = SoundOrWaveBankDestroyedNotificationCallback;

	hr = m_pEngine->Initialize( &xrParams );
	if (FAILED(hr))
	{
		Environment::get().pLogger->logMessage(
			"*** Could not initialize audio renderer instance ***");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool RendererXAudio2::destroy()
{
	if (m_pEngine)
	{
		for (Banks::iterator it = m_banks.begin(); it != m_banks.end(); ++it)
		{
			releaseBank(it->second);
		}

		m_pEngine->ShutDown();
	}

	return true;
}
//---------------------------------------------------------------------------
bool RendererXAudio2::update(float deltaT)
{
	if (m_pEngine)
		m_pEngine->DoWork();

	return true;
}
//---------------------------------------------------------------------------
Emitter* RendererXAudio2::createEmitter()
{
	return 0;
}
//---------------------------------------------------------------------------
void RendererXAudio2::releaseEmitter(Emitter* pEmitter)
{
}
//---------------------------------------------------------------------------
Listener* RendererXAudio2::createListener()
{
	return 0;
}
//---------------------------------------------------------------------------
void RendererXAudio2::releaseListener(Listener* pListener)
{
}
//---------------------------------------------------------------------------
Audio::Stream* RendererXAudio2::getStreamInstance(
	Handle hData, const String& name)
{
	return 0;
}
//---------------------------------------------------------------------------
Sample* RendererXAudio2::getSampleInstance(
	Handle hData, const String& name)
{
	if (hData)
	{
		Bank* pBank = (Bank*)hData;
		Bank::Sounds::iterator it = pBank->m_sounds.find(name);
		if (it != pBank->m_sounds.end())
			return it->second.m_pSample;
	}

	return 0;
}
//---------------------------------------------------------------------------
void RendererXAudio2::play2D(const Sample& sample)
{
}
//---------------------------------------------------------------------------
void RendererXAudio2::play2D(const Audio::Stream& stream)
{
}
//---------------------------------------------------------------------------
Handle RendererXAudio2::addOrRetrieveSound(const String& path)
{
	// first check to see if we already have one of these; if so, return 
	// the existing handle
	std::pair<Banks::iterator, bool> pr = m_banks.insert(
		Banks::value_type(path, Bank()));

	Bank& bank = pr.first->second;
	if (!pr.second)
		return &pr.first->second;

	// otherwise, take this opportunity to set the bank path
	bank.m_bankPath = path;

	// path should point to the location of .xsb/.xwb files; these files 
	// should be named the same as the last part of the containing path
	size_t pos = path.findLast('/');
	if (pos == String::INVALID)
		pos = path.findLast('\\');
	if (pos == String::INVALID)
		pos = 0;

	String xwb(path.substr(pos+1));
	String xsb(xwb);
	xwb += ".xwb";
	xsb += ".xsb";
	String xwbPath(path);
	xwbPath += "\\";
	xwbPath += xwb;
	String xsbPath(path);
	xsbPath += "\\";
	xsbPath += xsb;

	// get the size of the wav file; if it's more than 64K, memory-map it; 
	// otherwise, just load it into a memory buffer
	HANDLE hFile = CreateFile( 
		xwbPath, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL );

	if( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwSz = GetFileSize( hFile, NULL );
		if (dwSz > 65535)
		{
			HANDLE hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwSz, NULL );
			if( hMapFile )
			{
				bank.m_waveBank.m_pMappedView = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				if( bank.m_waveBank.m_pMappedView )
				{
					m_pEngine->CreateInMemoryWaveBank( 
						bank.m_waveBank.m_pMappedView, 
						dwSz, 
						0, 
						0, 
						&bank.m_waveBank.m_pWaveBank);
				}
				CloseHandle( hMapFile );
			}
		}
		else
		{
			// read contents of the file into memory
			DWORD dwBytesRead;
			bank.m_waveBank.m_buf.resize(dwSz);
			if (ReadFile(
				hFile, 
				&bank.m_waveBank.m_buf[0],
				dwSz,
				&dwBytesRead,
				NULL))
			{
				m_pEngine->CreateInMemoryWaveBank( 
					&bank.m_waveBank.m_buf[0], 
					dwSz, 
					0, 
					0, 
					&bank.m_waveBank.m_pWaveBank);
			}
		}
	}

	CloseHandle(hFile);

	// then load the sound bank -- this one is small enough just to go into memory
	hFile = CreateFile( xsbPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwSz = GetFileSize( hFile, NULL );
		if( dwSz != -1 )
		{
			bank.m_soundBank.m_buf.resize(dwSz);
			DWORD dwBytesRead;
			if( 0 != ReadFile( 
				hFile, 
				&bank.m_soundBank.m_buf[0], 
				dwSz, 
				&dwBytesRead, 
				NULL ) )
			{
				m_pEngine->CreateSoundBank( 
					&bank.m_soundBank.m_buf[0], 
					dwSz, 
					0, 
					0, 
					&bank.m_soundBank.m_pSoundBank);
			}

			// set up a notification for when we need to release the 
			// sound bank
			XACT_NOTIFICATION_DESCRIPTION desc = {0};
			desc.type = XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED;
			desc.pSoundBank = bank.m_soundBank.m_pSoundBank;
			bank.m_soundBank.hFreeEvent = 
				desc.pvContext = CreateEvent(NULL, TRUE, FALSE, NULL);
			m_pEngine->RegisterNotification(&desc);
		}
	}

	CloseHandle( hFile );

	// and then initialize the cue library
	XACTINDEX numCues;
	bank.m_soundBank.m_pSoundBank->GetNumCues(&numCues);
	for (XACTINDEX i=0; i<numCues; ++i)
	{
		XACT_CUE_PROPERTIES props;
		bank.m_soundBank.m_pSoundBank->GetCueProperties(i, &props);

		std::pair<Bank::Sounds::iterator, bool> pr =
			bank.m_sounds.insert(
				Bank::Sounds::value_type(props.friendlyName, Sound()));
		
		pr.first->second.m_index = i;
		pr.first->second.m_pSample = 
			COS_NEW SampleXAudio2(bank.m_soundBank.m_pSoundBank, i);

		// TODO: streaming?
	}

	return &pr.first->second;
}
//---------------------------------------------------------------------------
void RendererXAudio2::releaseSound(Handle hData)
{
	// hData is a pointer to a Bank; go through and delete/release all of the 
	// objects or memory we control in this bank
	Bank* pBank = (Bank*)hData;

	releaseBank(*pBank);

	Banks::iterator it = m_banks.find(pBank->m_bankPath);
	m_banks.erase(it);
}
//---------------------------------------------------------------------------
void RendererXAudio2::releaseBank(Bank& bank)
{
	// TODO: we should tell those that hold handles to us that we are leaving?
	for (Bank::Sounds::iterator it = bank.m_sounds.begin();
		it != bank.m_sounds.end(); ++it)
	{
		delete it->second.m_pSample;
		delete it->second.m_pStream;
	}

	// then destroy the sound and wave bank, and wait for them to quiesce
	// before releasing the underlying data
	bank.m_soundBank.m_pSoundBank->Destroy();
	bank.m_waveBank.m_pWaveBank->Destroy();

	HANDLE hWait[2];
	hWait[0] = bank.m_soundBank.hFreeEvent;
	hWait[1] = bank.m_waveBank.hFreeEvent;
	WaitForMultipleObjects(2, hWait, TRUE, INFINITE);

	// now it is safe to release underlying data (buffers clean themselves up
	// when we delete them from the Banks map)
	if (bank.m_waveBank.m_pMappedView)
		UnmapViewOfFile(bank.m_waveBank.m_pMappedView);
}
