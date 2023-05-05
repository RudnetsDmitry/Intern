// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#undef ASSERT
#undef VERIFY

#include <crtdbg.h>

#define ASSERT(f) _ASSERTE(f)
#ifdef _DEBUG
#define VERIFY(f) ASSERT(f)
#else
#define VERIFY(f) ((void)(f))
#endif




// reference additional headers your program requires here
