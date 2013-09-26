#if !defined(LIBHFZ_WRAPPER_INCLUDED)
#define LIBHFZ_WRAPPER_INCLUDED

__declspec(dllexport) float* hfzLoadHeightfield(const char* filename);
__declspec(dllexport) void hfzFreeHeightfieldData(float* data);

#endif // LIBHFZ_WRAPPER_INCLUDED
