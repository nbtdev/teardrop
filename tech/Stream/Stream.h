/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDSTREAM_INCLUDED)
#define TDSTREAM_INCLUDED

namespace Teardrop
{
	class Stream
	{
	public:
		typedef enum
		{
			BEGIN,
			CURRENT,
			END
		} SeekType;

		virtual ~Stream();

		//! read data from this stream
		virtual bool read(void* pData, size_t len, bool async=false) = 0;
		//! write data to this stream
		virtual bool write(const void* pData, size_t len, bool async=false) = 0;
		//! return stream length
		virtual size_t length() = 0;
		//! return stream position
		virtual size_t getPosition() = 0;
		//! seek to another part of this stream
		virtual bool seek(int offset, SeekType seekType = CURRENT, bool async=false) = 0;
		//! check to see if we are at the end of the stream
		virtual bool isEnd() = 0;

		Stream& operator<<(const char*);
		Stream& operator<<(int v);
		Stream& operator<<(unsigned int v);
		Stream& operator<<(bool v);

		Stream& operator>>(int& v);
		Stream& operator>>(unsigned int& v);
		Stream& operator>>(bool& v);

		static const char* ENDL;
		static const char* TAB;
	};

}

#endif // TDSTREAM_INCLUDED
