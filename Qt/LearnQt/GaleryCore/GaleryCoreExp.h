#pragma once

/*
Qt variant

#include <QtCore/qglobal.h>
#if defined(GALERYCORE_EXPORTS)
# define GALLERYCORESHARED_EXPORT Q_DECL_EXPORT
#else
# define GALLERYCORESHARED_EXPORT Q_DECL_IMPORT
#endif

*/

#ifndef GALERYCORE_EXPORTS
#define GALERYCORE_EXT_CLASS __declspec(dllimport)
#define GALERYCORE_EXT_DATA __declspec(dllimport)
#else 
#define GALERYCORE_EXT_CLASS __declspec(dllexport)
#define GALERYCORE_EXT_DATA extern __declspec(dllexport)
#endif
