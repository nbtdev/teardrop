/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#if !defined(GFX_EXCEPTION_INCLUDED)
#define GFX_EXCEPTION_INCLUDED

#include "Util/_String.h"
#include <exception>

namespace Teardrop
{
	namespace Gfx
	{
		class Exception : public std::exception
		{
		public:
			Exception(const char* aDetail);
			Exception(const String& aDetail);
            ~Exception() noexcept;

            const char* what() const noexcept;

		protected:
			String mMsg;
		};

		class InvalidParameterException : public Exception
		{
		public:
			InvalidParameterException(const char* aDetail);
			InvalidParameterException(const String& aDetail);
            ~InvalidParameterException() noexcept;
		};

		class BufferMappedException : public Exception
		{
		public:
			BufferMappedException(const char* aDetail, void* aMappedBuffer);
			BufferMappedException(const String& aDetail, void* aMappedBuffer);
            ~BufferMappedException() noexcept;
			void* mappedBuffer() const;

		protected:
			void* mMappedBuffer = nullptr;
		};

		class ShaderCompilationException : public Exception
		{
		public:
			ShaderCompilationException(const char* aDetail, const String& aSource, const String& aLog);
			ShaderCompilationException(const String& aDetail, const String& aSource, const String& aLog);
            ~ShaderCompilationException() noexcept;

			const String& source() const;
			const String& log() const;

		protected:
			String mSource;
			String mLog;
		};
	}
}

#endif // GFX_EXCEPTION_INCLUDED
