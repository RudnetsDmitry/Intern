#pragma once

/*
Qt variant

#include <QtCore/qglobal.h>
#if defined(IMAGEPROCESS_EXPORTS)
# define IMAGEPROCESS_EXPORT Q_DECL_EXPORT
#else
# define IMAGEPROCESS_EXPORT Q_DECL_IMPORT
#endif

*/

#ifndef VSG_EXPORTS
#define VSG_EXT_CLASS __declspec(dllimport)
#define VSG_EXT_DATA __declspec(dllimport)
#else 
#define VSG_EXT_CLASS __declspec(dllexport)
#define VSG_EXT_DATA extern __declspec(dllexport)
#endif
