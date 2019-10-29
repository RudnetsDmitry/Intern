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

#ifndef GALERYGUI_EXPORTS
#define GALERYGUI_EXT_CLASS __declspec(dllimport)
#define GALERYGUI_EXT_DATA __declspec(dllimport)
#else 
#define GALERYGUI_EXT_CLASS __declspec(dllexport)
#define GALERYGUI_EXT_DATA extern __declspec(dllexport)
#endif

