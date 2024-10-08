/// @file	ExportFunctions.cpp
///
/// @see	ExportFunctions.h
///
/// @author	RDN © Intern :)
///
/// @date    22.05.2024 22:24
///

#include "stdafx.h"
#include "ExportFunctions.h"

#include "MainWindow.h"

namespace gallery
{
	QMainWindow * MakekMandelbrotWindow()
	{
		return new MainWindow();
	}
}
