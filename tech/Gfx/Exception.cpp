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

#include "Exception.h"

namespace Teardrop {
namespace Gfx {

Exception::Exception(const char* aDetail)
	: mMsg(aDetail)
{

}

Exception::Exception(const String& aDetail)
	: mMsg(aDetail)
{

}

Exception::~Exception() noexcept
{

}

const char* Exception::what() const noexcept
{
	return mMsg;
}

InvalidParameterException::InvalidParameterException(const char* aDetail)
	: Exception(aDetail)
{

}

InvalidParameterException::InvalidParameterException(const String& aDetail)
	: Exception(aDetail)
{

}

InvalidParameterException::~InvalidParameterException()
{

}

BufferMappedException::BufferMappedException(const char* aDetail, void* aMappedBuffer)
	: Exception(aDetail)
	, mMappedBuffer(aMappedBuffer)
{

}

BufferMappedException::BufferMappedException(const String& aDetail, void* aMappedBuffer)
	: Exception(aDetail)
	, mMappedBuffer(aMappedBuffer)
{

}

BufferMappedException::~BufferMappedException()
{

}

void* BufferMappedException::mappedBuffer() const
{
	return mMappedBuffer;
}

ShaderCompilationException::ShaderCompilationException(const char* aDetail, const String& aSource, const String& aLog)
	: Exception(aDetail)
	, mSource(aSource)
	, mLog(aLog)
{

}

ShaderCompilationException::ShaderCompilationException(const String& aDetail, const String& aSource, const String& aLog)
	: Exception(aDetail)
	, mSource(aSource)
	, mLog(aLog)
{

}

ShaderCompilationException::~ShaderCompilationException()
{

}

const String& ShaderCompilationException::source() const
{
	return mSource;
}

const String& ShaderCompilationException::log() const
{
	return mLog;
}

} // namespace Gfx
} // namespace Teardrop
