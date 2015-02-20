/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "Logger.h"
#include "Stream/FileStream.h"
#include "Memory/Memory.h"
#include <string.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
Logger::Logger(Stream& logStream)
	: m_logStream(logStream)
{
}
//---------------------------------------------------------------------------
Logger::~Logger()
{
}
//---------------------------------------------------------------------------
void Logger::logMessage(const char* message)
{
	m_logStream.write((void*)message, strlen(message));
	m_logStream.write("\r\n", 2);
}
