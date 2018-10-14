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

#if !defined(ASSETIMPORTEXCEPTION_INCLUDED)
#define ASSETIMPORTEXCEPTION_INCLUDED

#include "Config.h"

#include <exception>
#include <string>

namespace Teardrop {
	namespace Tools {

		class AssetImportException :
			public std::exception
		{
		public:
			AssetImportException(const std::string& aAssetPath);
            ~AssetImportException() noexcept;

            const char* what() const noexcept;
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
            ~AssetFileNotFoundException() noexcept;
		};

		class InvalidAssetFormatException : public AssetImportException
		{
		public:
			InvalidAssetFormatException(const std::string& aAssetPath, const std::string& aDetail);
            ~InvalidAssetFormatException() noexcept;
		};
	}
}

#endif // ASSETIMPORTEXCEPTION_INCLUDED
