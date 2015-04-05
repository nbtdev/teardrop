/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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

Exception::~Exception()
{

}

const char* Exception::what() const
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

} // namespace Gfx
} // namespace Teardrop