/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(THREADUTIL_INCLUDED)
#define THREADUTIL_INCLUDED

#if !defined(WIN32)
#pragma error("This header is valid for Win32 only!!")
#endif // WIN32

namespace Teardrop
{
	#define MS_VC_EXCEPTION 0x406D1388

	#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
	#pragma pack(pop)

	void SetThreadName( DWORD dwThreadID, char* threadName);
}

#endif // THREADUTIL_INCLUDED
