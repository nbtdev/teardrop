/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFX_EXCEPTION_INCLUDED)
#define GFX_EXCEPTION_INCLUDED

#include "Util/_String.h"
#include <exception>

#if defined(_WIN32) || defined(_WIN64)
	#define NOTHROW _NOEXCEPT
#else // _WIN32, _WIN64
	#define NOTHROW nothrow
#endif // _WIN32, _WIN64

namespace Teardrop
{
	namespace Gfx
	{
		class Exception : public std::exception
		{
		public:
			Exception(const char* aDetail);
			Exception(const String& aDetail);
			~Exception() NOTHROW;

			const char* what() const NOTHROW;

		protected:
			String mMsg;
		};

		class InvalidParameterException : public Exception 
		{
		public:
			InvalidParameterException(const char* aDetail);
			InvalidParameterException(const String& aDetail);
			~InvalidParameterException() NOTHROW;
		};
	}
}

#endif // GFX_EXCEPTION_INCLUDED
