// StreamTest.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string>

#include "Stream/FileStream.h"
#include "Stream/MemoryStream.h"
#include "Stream/TextReader.h"

using namespace CoS;

const char* phrases[] =
{
	"",
	"the quick brown fox",
	"jumped over the",
	"",
	"lazy dog",
	""
};

int textReaderTest(const char* fname, const std::string& test)
{
	const int len = 1024;
	char buf[len];

	fprintf(stdout, "File %s...", fname);

	FileStream fs;
	if (!fs.open(fname, READ))
	{
		fprintf(stderr, "Missing input file %s\n", fname);
		return 1;
	}

	TextReader r(fs);
	int phrase = 0;
	while (r.getline(buf, len))
	{
		if (std::string(phrases[phrase++]) != buf)
		{
			fprintf(stderr, "FAILED\n");
			return 3;
		}
	}

	fs.close();

	if (test != buf)
	{
		fprintf(stderr, "FAILED\n");
		return 2;
	}

	fprintf(stdout, "SUCCEEDED\n");

	return 0;
}

const char* fnames[] =
{
	"dos.txt", 
	"dos2.txt",
	"unix.txt",
	"unix2.txt"
};

std::string test1("lazy dog");
std::string test2("");

const std::string* tests[] =
{
	&test1,
	&test2, 
	&test1,
	&test2
};

int main(int argc, char* argv[])
{
	fprintf(stdout, "\nTEXT READER TEST\n\n");

	int numTests = 4;

	for (int i=0; i<numTests; ++i)
	{
		int rtn = textReaderTest(fnames[i], *(tests[i]));
		if (rtn)
		{
			return rtn;
		}
	}

	return 0;
}

