#include "libhfz.h"

DLLEXPORT float* hfzLoadHeightfield(const char* filename)
{
	float* rtn = 0;
	hfzHeader hdr;
	long status = hfzLoadEx(filename, hdr, &rtn, 0, 0);
	return rtn;
}

DLLEXPORT void hfzFreeHeightfieldData(float* data)
{
	if (data)
		hfzFree(data);
}
