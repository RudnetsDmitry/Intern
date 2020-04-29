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

#ifndef QMLGUI_EXPORTS
#define QMLGUI_EXT_CLASS __declspec(dllimport)
#define QMLGUI_EXT_DATA __declspec(dllimport)
#else 
#define QMLGUI_EXT_CLASS __declspec(dllexport)
#define QMLGUI_EXT_DATA extern __declspec(dllexport)
#endif
