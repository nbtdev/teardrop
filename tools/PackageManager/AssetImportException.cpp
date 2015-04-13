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

#include "AssetImportException.h"

namespace Teardrop {
namespace Tools {

	AssetImportException::AssetImportException(const std::string& aAssetPath)
		: mAssetPath(aAssetPath)
	{
	}


	AssetImportException::~AssetImportException() NOTHROW
	{
	}

	const char* AssetImportException::what() const NOTHROW
	{
		return mDetail.c_str();
	}

	const char* AssetImportException::detail() const
	{
		return mDetail.c_str();
	}

	const char* AssetImportException::assetPath() const
	{
		return mAssetPath.c_str();
	}

	AssetFileNotFoundException::AssetFileNotFoundException(const std::string& aAssetPath)
		: AssetImportException(aAssetPath)
	{
		mDetail = "File not found";
	}

	AssetFileNotFoundException::~AssetFileNotFoundException() NOTHROW
	{
	}

	InvalidAssetFormatException::InvalidAssetFormatException(const std::string& aAssetPath, const std::string& aDetail)
		: AssetImportException(aAssetPath)
	{
		mDetail = aDetail;
	}

	InvalidAssetFormatException::~InvalidAssetFormatException() NOTHROW
	{
	}

} // namespace Tools
} // namespace Teardrop

