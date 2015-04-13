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
