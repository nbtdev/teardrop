/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
		return (std::string("Exception occurred importing asset: ") + mDetail).c_str();
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

