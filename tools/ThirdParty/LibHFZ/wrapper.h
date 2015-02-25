#if !defined(LIBHFZ_WRAPPER_INCLUDED)
#define LIBHFZ_WRAPPER_INCLUDED

#if defined(_WIN32) || defined(_WIN64)
    #define DLLEXPORT __declspec(dllexport)
#else // _WIN32, _WIN64
    #define DLLEXPORT
#endif // _WIN32, _WIN64

DLLEXPORT float* hfzLoadHeightfield(const char* filename);
DLLEXPORT void hfzFreeHeightfieldData(float* data);

#endif // LIBHFZ_WRAPPER_INCLUDED
