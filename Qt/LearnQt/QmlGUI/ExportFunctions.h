/// @file	ExportFunctions.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   06.12.2019   17:45
///

#pragma once

#include "QmlGUIExp.h"
#include <memory>

class QMainWindow;
class QGuiApplication;

namespace qml_gui
{
	QMLGUI_EXT_CLASS QMainWindow* MakeQmlWindow();
	QMLGUI_EXT_CLASS std::unique_ptr<QGuiApplication> MakeQmlApp(int argc, char* argv[]);
}
