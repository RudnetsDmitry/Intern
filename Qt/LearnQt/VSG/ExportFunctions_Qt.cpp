/// @file	VulkanWindow.cpp
///
/// @see	VulkanWindow.h
///
/// @author	RDN © Intern :)
///
/// @date    26.02.2025 21:58
///

#include "stdafx.h"

#include "ExportFunctions.h"

#include <QtCore/QString>

#include "Qt/vulkanwindow.h"
#include "Qt/mainwindow.h"
#include <QLoggingCategory>
#include <QMainWindow>

namespace gallery
{
	QVulkanInstance inst;

	QMainWindow* MakeVSGWindow__()
	{
		const bool dbg = qEnvironmentVariableIntValue("QT_VK_DEBUG");

		if (dbg)
		{
			QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));
			inst.setLayers({ "VK_LAYER_KHRONOS_validation" });
		}

		if (!inst.create())
			qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

		VulkanWindow* vulkanWindow = new VulkanWindow(dbg);
		vulkanWindow->setVulkanInstance(&inst);

		MainWindow * mainWindow = new MainWindow(vulkanWindow);
		QMainWindow* wnd = new QMainWindow();
		wnd->setCentralWidget(mainWindow);
		wnd->resize(1024, 768);

		return wnd;
	}
}
