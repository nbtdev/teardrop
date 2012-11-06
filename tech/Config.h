/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TD_CONFIG_INCLUDED)
#define TD_CONFIG_INCLUDED

#if !defined(TD_TARGET_DEBUG) && !defined(TD_TARGET_RELEASE) && !defined(TD_TARGET_FINAL)

	#if defined(_DEBUG)
	#	define TD_TARGET_DEBUG
	#elif defined(NDEBUG)
	#	define TD_TARGET_RELEASE
	#else
	#	define TD_TARGET_FINAL
	#endif

#endif



/*
	TD_OPTION_MEMPROFILE

	Enable memory allocation tracking
*/

#if defined(TD_TARGET_DEBUG)
	//#define TD_OPTION_MEMPROFILE
#elif defined(TD_TARGET_RELEASE)
	//#define TD_OPTION_MEMPROFILE
#elif defined(TD_TARGET_FINAL)
	//#define TD_OPTION_MEMPROFILE
#else
	#error ("No target defined!")
#endif

#endif // TD_CONFIG_INCLUDED
