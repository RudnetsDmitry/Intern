// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define NOMINMAX

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#if defined(WIN32)
#    define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__APPLE__)
#    define VK_USE_PLATFORM_MACOS_MVK
#else
#    define VK_USE_PLATFORM_XCB_KHR
// #define VK_USE_PLATFORM_XLIB_KHR
#endif



// reference additional headers your program requires here
