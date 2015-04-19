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

#include "ShaderRealizer.h"
#include "Argument.h"
#include "Stream/Stream.h"

namespace Teardrop {
namespace Gfx {
namespace Codegen {

ShaderRealizer::Token::Token(TokenType aType) 
	: mType(aType) 
{

}

ShaderRealizer::Token::Token(TokenType aType, const String& aVal)
	: mType(aType)
	, mVal(aVal)
{

}

ShaderRealizer::Token::Token(TokenType aType, DataType aDataType)
	: mType(aType)
	, mDataType(aDataType)
{

}

ShaderRealizer::Token::~Token()
{

}

ShaderRealizer::ShaderRealizer()
{

}

ShaderRealizer::~ShaderRealizer()
{

}

void ShaderRealizer::openScope()
{
	mTokens.push_back(Token(TOKEN_LBRACE));
}

void ShaderRealizer::closeScope()
{
	mTokens.push_back(Token(TOKEN_RBRACE));
}

void ShaderRealizer::endStatement()
{
	mTokens.push_back(Token(TOKEN_SEMICOLON));
}

void ShaderRealizer::insertFunctionPrologue(DataType aReturnType, const String& aName, Argument const** aArguments, int aArgCount)
{
	// something like 
	//     void Foo(in type arg1, inout type arg2, out type arg3)
	mTokens.push_back(Token(TOKEN_TYPE, aReturnType));
	mTokens.push_back(Token(TOKEN_LITERAL, aName));
	mTokens.push_back(Token(TOKEN_LPAREN));

	for (int i = 0; i < aArgCount; ++i) {
		if (i) mTokens.push_back(Token(TOKEN_COMMA));

		switch (aArguments[i]->direction()) {
		case DIRECTION_IN: mTokens.push_back(Token(TOKEN_IN)); break;
		case DIRECTION_OUT: mTokens.push_back(Token(TOKEN_OUT)); break;
		case DIRECTION_INOUT: mTokens.push_back(Token(TOKEN_INOUT)); break;
		default: mTokens.push_back(Token(TOKEN_IN)); break;
		}

		mTokens.push_back(Token(TOKEN_TYPE, aArguments[i]->type()));
		mTokens.push_back(Token(TOKEN_LITERAL, aArguments[i]->name()));
	}

	mTokens.push_back(Token(TOKEN_RPAREN));
}

void ShaderRealizer::insertBinaryOperation(BinaryOperation /*aOperation*/)
{

}

void ShaderRealizer::insertCast(DataType /*aDestType*/, Expression::ConstRef /*aSourceExpr*/)
{

}

void ShaderRealizer::insertModifier(Direction /*aDirection*/)
{

}

void ShaderRealizer::insertDataType(DataType /*aType*/)
{

}

void ShaderRealizer::insertIdentifier(const String& /*aName*/)
{

}

void ShaderRealizer::insertTextureSample(const String& /*aSamplerIdentifier*/, const Sampler2D* /*aSampler*/)
{

}

namespace {
	const char* sTypes[] = {
		"void",            // TYPE_VOID,
		"float",           // TYPE_FLOAT,
		"float2",          // TYPE_FLOAT2,
		"float3",          // TYPE_FLOAT3,
		"float4",          // TYPE_FLOAT4,
		"void",            // TYPE_UNKNOWN,
	};
}


void ShaderRealizer::realize(Stream& aStream) const
{
	const char* tabs = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	const char* LF ="\n";
	int currentIndent = 0;

	for (auto& t : mTokens) {
		switch (t.mType) {
		case TOKEN_TYPE: aStream.write(sTypes[t.mDataType], strlen(sTypes[t.mDataType])); aStream.write(" ", 1);  break;
		case TOKEN_ASSIGN: aStream.write(" = ", 3); break;
		case TOKEN_EQ: aStream.write(" == ", 4); break;
		case TOKEN_NEQ: aStream.write(" != ", 4); break;
		case TOKEN_LT: aStream.write(" < ", 3); break;
		case TOKEN_GT: aStream.write(" > ", 3); break;
		case TOKEN_MULTIPLY: aStream.write(" * ", 3); break;
		case TOKEN_ADD: aStream.write(" + ", 3); break;
		case TOKEN_SUBTRACT: aStream.write(" - ", 3); break;
		case TOKEN_DIVIDE: aStream.write(" / ", 3); break;
		case TOKEN_COMMA: aStream.write(",", 1); break;
		case TOKEN_SEMICOLON: aStream.write(";", 1); aStream.write(LF, 1); if (currentIndent) aStream.write(tabs, currentIndent); break;
		case TOKEN_COLON: aStream.write(":", 1); break;
		case TOKEN_LPAREN: aStream.write(" ( ", 3); break;
		case TOKEN_RPAREN: aStream.write(" ) ", 3); break;
		case TOKEN_LBRACE: aStream.write("{", 1); aStream.write(LF, 1); currentIndent++; if (currentIndent > 0) aStream.write(tabs, currentIndent); break;
		case TOKEN_RBRACE: 
			aStream.write(LF, 1); 
			currentIndent--; 
			if (currentIndent > 0) aStream.write(tabs, currentIndent); 
			aStream.write("}", 1); 
			aStream.write(LF, 1); 
			currentIndent--; 
			if (currentIndent > 0) aStream.write(tabs, currentIndent); 
			break;
		case TOKEN_LBRACKET: aStream.write(" ] ", 3); break;
		case TOKEN_RBRACKET: aStream.write(" ] ", 3); break;
		case TOKEN_IN: aStream.write(" in ", 4); break;
		case TOKEN_OUT: aStream.write(" out ", 5); break;
		case TOKEN_INOUT: aStream.write(" inout ", 7); break;
		case TOKEN_LITERAL: aStream.write((const char*)t.mVal, t.mVal.length()); break;
		default: break;
		}
	}
}

} // namespace Codegen
} // namespace Gfx
} // namespace Teardrop
