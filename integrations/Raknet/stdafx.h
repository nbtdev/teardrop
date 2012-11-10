#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

// TODO: reference additional headers your program requires here
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "Bitstream.h"
#include "GetTime.h"