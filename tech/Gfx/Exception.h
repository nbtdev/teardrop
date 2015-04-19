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

#if defined(_WIN32) || defined(_WIN64)
	#define NOTHROW _NOEXCEPT

	#if defined(min)
		#undef min
	#endif // min

	#if defined(max)
		#undef max
	#endif // min
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
			explicit Exception(const char* aDetail);
			explicit Exception(const String& aDetail);
			~Exception() NOTHROW;

			const char* what() const NOTHROW;

		protected:
			String mMsg;
		};

		class InvalidParameterException : public Exception
		{
		public:
			explicit InvalidParameterException(const char* aDetail);
			explicit InvalidParameterException(const String& aDetail);
			~InvalidParameterException() NOTHROW;
		};

		class IndexOutOfRangeException : public Exception
		{
		public:
			explicit IndexOutOfRangeException(const char* aDetail, int aMin, int aMax, int aHave);
			explicit IndexOutOfRangeException(const String& aDetail, int aMin, int aMax, int aHave);
			~IndexOutOfRangeException() NOTHROW;

			int min() const;
			int max() const;
			int have() const;

		private:
			int mMin;
			int mMax;
			int mHave;
		};

		class BufferMappedException : public Exception
		{
		public:
			explicit BufferMappedException(const char* aDetail, void* aMappedBuffer);
			explicit BufferMappedException(const String& aDetail, void* aMappedBuffer);
			~BufferMappedException() NOTHROW;
			void* mappedBuffer() const;

		protected:
			void* mMappedBuffer = nullptr;
		};

		class ShaderCompilationException : public Exception
		{
		public:
			explicit ShaderCompilationException(const char* aDetail, const String& aSource, const String& aLog);
			explicit ShaderCompilationException(const String& aDetail, const String& aSource, const String& aLog);
			~ShaderCompilationException() NOTHROW;

			const String& source() const;
			const String& log() const;

		protected:
			String mSource;
			String mLog;
		};
	}
}

#endif // GFX_EXCEPTION_INCLUDED
