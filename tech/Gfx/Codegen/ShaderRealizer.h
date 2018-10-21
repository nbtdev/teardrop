/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(TEARDROP_GFX_CODEGEN_SHADERREALIZER_INCLUDED)
#define TEARDROP_GFX_CODEGEN_SHADERREALIZER_INCLUDED

#include "Gfx/Codegen/Common.h"
#include "Gfx/Codegen/Expression.h"
#include "Util/_String.h"
#include <list>
#include <vector>

namespace Teardrop
{
	class String;
	class Stream;

	namespace Gfx
	{
		class Sampler2D;

		namespace Codegen
		{
			class Argument;

			class ShaderRealizer
			{
			public:
				ShaderRealizer();
				~ShaderRealizer();

                virtual void beginProgram();
                virtual void endProgram();
				virtual void openScope();
				virtual void closeScope();
				virtual void endStatement();
				virtual void insertFunctionPrologue(DataType aReturnType, const String& aName, Argument const** aArguments, int aArgCount);
				virtual void insertBinaryOperation(BinaryOperation aOperation);
				virtual void insertCast(DataType aDestType);

				// For example, a swizzle of ".r" would be "aComponents=[1]" and "aNumComponents=1". 
				// .xyxx would be "aComponents=[1,2,1,1]" and "aNumComponents=4". .rgb would be
				// "aComponents=[1,2,3]" and "aNumComponents=3". And so on...
				virtual void insertSwizzle(const int* aComponents, int aNumComponents);

				virtual void insertModifier(Direction aDirection);
				virtual void insertDataType(DataType aType);
				virtual void insertIdentifier(const String& aName);
				virtual void insertTextureSample(const String& aSamplerIdentifier, const Sampler2D* aSampler);

				void realize(Stream& aStream) const;

			protected:

				enum TokenType
				{
					TOKEN_INVALID,
					TOKEN_TYPE,
					TOKEN_IN,
					TOKEN_OUT,
					TOKEN_INOUT,
					TOKEN_LBRACE,
					TOKEN_RBRACE,
					TOKEN_LPAREN,
					TOKEN_RPAREN,
					TOKEN_LBRACKET,
					TOKEN_RBRACKET,
					TOKEN_IDENTIFIER,
					TOKEN_LITERAL,
					TOKEN_ASSIGN,
					TOKEN_EQ,
					TOKEN_NEQ,
					TOKEN_LT,
					TOKEN_GT,
					TOKEN_MULTIPLY,
					TOKEN_ADD,
					TOKEN_SUBTRACT,
					TOKEN_DIVIDE,
					TOKEN_COMMA,
					TOKEN_SEMICOLON,
					TOKEN_COLON,
					TOKEN_PERIOD,
					TOKEN_SWIZZLE
				};

				struct Token {
					Token(TokenType aType);
					Token(TokenType aType, const String& aVal);
					Token(TokenType aType, DataType aDataType);
					~Token();

					TokenType mType = TOKEN_INVALID;
					DataType mDataType = TYPE_UNKNOWN;
					String mVal;
					char mSwizzle[5];
				};

				std::list<Token> mTokens;

				virtual void startingRealize(Stream& aStream) const;
				virtual void endedRealize(Stream& aStream) const;

				virtual const char* lookupTypeString(DataType aType) const;
			};
		} // namespace Codegen
	} // namespace Gfx
} // namespace Teardrop


#endif // TEARDROP_GFX_CODEGEN_SHADERREALIZER_INCLUDED
