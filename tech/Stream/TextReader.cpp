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
