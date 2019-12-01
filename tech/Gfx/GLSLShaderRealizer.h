/******************************************************************************
Copyright (c) 2019 Teardrop Games

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

#pragma once

#include "Gfx/Codegen/ShaderRealizer.h"

namespace Teardrop {
namespace Gfx {
namespace Codegen {

class Argument;

class GLSLShaderRealizer : public ShaderRealizer
{
public:
    GLSLShaderRealizer();
    ~GLSLShaderRealizer();

    // Codegen::ShaderRealizer implementation
    void beginProgram() override;
    void endProgram() override;
    void openScope() override;
    void closeScope() override;
    void endStatement() override;
    void insertFunctionPrologue(DataType aReturnType, const String& aName, Argument const** aArguments, int aArgCount) override;
    void insertBinaryOperation(BinaryOperation aOperation) override;
    void insertCast(DataType aDestType) override;
    void insertSwizzle(const int* aComponents, int aNumComponents) override;
    void insertModifier(Direction aDirection) override;
    void insertDataType(DataType aType) override;
    void insertIdentifier(const String& aName) override;
    void insertTextureSample(const String& aSamplerIdentifier, const Sampler2D* aSampler) override;

protected:
    void startingRealize(Stream& aStream) const override;
    void endedRealize(Stream& aStream) const override;
    const char* lookupTypeString(DataType aType) const override;
};

} // namespace Codegen
} // namespace Gfx
} // namespace Teardrop
