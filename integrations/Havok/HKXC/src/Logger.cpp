/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include "Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static const char* FATAL_PREFIX = "!!FATAL: ";
static const char* ERR_PREFIX = "!!ERROR: ";
static const char* NO_PREFIX = "";
//---------------------------------------------------------------------------
void Logger::logMessage(
	Verbosity level, 
	const char* toolName, 
	const char* msg, 
	...)
{
	va_list(arglist);
	va_start(arglist, msg);

	FILE* fpOut = 0;
	const char* prefix = NO_PREFIX;

	switch (level)
	{
	case SILENT:
		break;

	case FATAL:
		prefix = FATAL_PREFIX;
		fpOut = stderr;
		break;

	case ERR:
		prefix = ERR_PREFIX;
		fpOut = stderr;
		break;

	case WARNING:
	case INFO:
	case BORE_ME:
		fpOut = stdout;
		break;
	}

	if (fpOut)
	{
		fprintf(fpOut, "[%s] %s", toolName, prefix);
		vfprintf(fpOut, msg, arglist);
		fprintf(fpOut, "\n");
	}
}