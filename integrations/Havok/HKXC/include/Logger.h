/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
