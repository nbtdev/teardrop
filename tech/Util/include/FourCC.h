/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COSFOURCC_INCLUDED)
#define COSFOURCC_INCLUDED

namespace CoS
{
	typedef union
	{
		size_t wordVal;
		char charVal[4];
	} _FourCC;

	struct FourCC
	{
		_FourCC fourCC;

		FourCC();
		~FourCC();
		FourCC(char a, char b, char c, char d);
		bool operator==(const FourCC& other) const;
		bool operator!=(const FourCC& other) const;
		bool operator<(const FourCC& other) const;
		FourCC& operator=(const FourCC& other);
	};

	inline FourCC::FourCC()
	{
		fourCC.wordVal = 0x20202020; // binary of four spaces
	}

	inline FourCC::FourCC(char a, char b, char c, char d)
	{
		fourCC.charVal[0] = a;
		fourCC.charVal[1] = b;
		fourCC.charVal[2] = c;
		fourCC.charVal[3] = d;
	}

	inline bool FourCC::operator ==(const FourCC& other) const
	{
		return (fourCC.wordVal == other.fourCC.wordVal);
	}

	inline bool FourCC::operator !=(const FourCC& other) const
	{
		return !(*this == other);
	}

	inline bool FourCC::operator <(const FourCC& other) const
	{
		return (fourCC.wordVal < other.fourCC.wordVal);
	}

	inline FourCC& FourCC::operator =(const FourCC& other)
	{
		fourCC.wordVal = other.fourCC.wordVal;
		return *this;
	}
}

#endif // COSFOURCC_INCLUDED
