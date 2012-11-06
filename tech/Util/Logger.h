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
	class Stream; 

	class Logger
	{
		Stream& m_logStream;

	public:
		Logger(Stream& logStream);
		virtual ~Logger();

		//! write text message to the log stream
		void logMessage(const char* message);
		//! convenience getter
		Stream& getLogStream() { return m_logStream; }

	private:
		Logger(const Logger& other);
		Logger& operator=(const Logger& other);
	};
}

#endif // TDLOGGER_INCLUDED
