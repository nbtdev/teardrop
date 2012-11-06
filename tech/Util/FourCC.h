/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDFOURCC_INCLUDED)
#define TDFOURCC_INCLUDED

namespace Teardrop
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

#endif // TDFOURCC_INCLUDED
