/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ASSETIMPORTEXCEPTION_INCLUDED)
#define ASSETIMPORTEXCEPTION_INCLUDED

#include <exception>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
	#define NOTHROW _NOEXCEPT
#else
	#define NOTHROW nothrow
#endif

namespace Teardrop {
	namespace Tools {

		class AssetImportException :
			public std::exception
		{
		public:
			AssetImportException(const std::string& aAssetPath);
			~AssetImportException() NOTHROW;

			const char* what() const NOTHROW;
			const char* detail() const;
			const char* assetPath() const;

		protected:
			std::string mAssetPath;
			std::string mDetail;
		};

		class AssetFileNotFoundException : public AssetImportException
		{
		public:
			AssetFileNotFoundException(const std::string& aAssetPath);
			~AssetFileNotFoundException() NOTHROW;
		};

		class InvalidAssetFormatException : public AssetImportException
		{
		public:
			InvalidAssetFormatException(const std::string& aAssetPath, const std::string& aDetail);
			~InvalidAssetFormatException() NOTHROW;
		};
	}
}

#endif // ASSETIMPORTEXCEPTION_INCLUDED
