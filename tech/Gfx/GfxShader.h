/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXSHADER_INCLUDED)
#define GFXSHADER_INCLUDED

namespace Teardrop
{
	class String;

	class GfxShader
	{
	public:
		GfxShader();
		virtual ~GfxShader();

		virtual bool compile(const String& source, String& errs) = 0;
		virtual const String& getSource() const = 0;
		virtual const String& getCompileErrors() const = 0;
		virtual const String& getFullPath() const = 0;
	};
}

#endif // GFXSHADER_INCLUDED
