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

