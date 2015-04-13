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

#if !defined(TDLOGGER_INCLUDED)
#define TDLOGGER_INCLUDED

namespace Teardrop
{
	class Logger
	{
	public:

		typedef enum
		{
			SILENT,
			FATAL,
			ERR,
			WARNING,
			INFO,
			BORE_ME,
		} Verbosity;

		//! log a message with the given verbosity to stdout/stderr
		static void logMessage(Verbosity level, const char* toolName, const char* msg, ...);
		
		/** accessors
		*/

		/** mutators
		*/

	private:
		//! NOT IMPLEMENTED
		Logger(const Logger& other);
		Logger& operator=(const Logger& other);
		Logger();
		~Logger();
	};
}

#endif // TDLOGGER_INCLUDED
