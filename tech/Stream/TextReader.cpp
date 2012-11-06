/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "TextReader.h"
#include "Stream/Stream.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TextReader::TextReader(Stream& s)
: m_strm(s)
{
}
//---------------------------------------------------------------------------
TextReader::~TextReader()
{
}
//---------------------------------------------------------------------------
bool TextReader::getline(char* buf, size_t len)
{
	// read from the current stream location up to (but not including) any 
	// line-end character sequence
	size_t charsRead = 0;

	while (!m_strm.isEnd() && charsRead < len)
	{
		char c;
		if (!m_strm.read(&c, 1))
		{
			// then end of file reached, stick in a null and return true
			// if anything was read
			if (charsRead > 0)
			{
				*buf = 0;
				return true;
			}
			break;
		}
		charsRead++;

		if (c == '\r' || c == '\n' || !c)
		{
			// end of either string or line reached
			*buf = 0;

			// check to see if there is another char in the newline sequence
			if (!m_strm.isEnd() && c == '\r')
			{
				m_strm.read(&c, 1);
				if (!m_strm.isEnd())
				{
					if (c != '\n' && c)
					{
						m_strm.seek(-1);
					}
				}
			}
			return true;
		}
		else
		{
			// append to the output buffer
			*(buf++) = c;
		}
	}

	return false;
}
