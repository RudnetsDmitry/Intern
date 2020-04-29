/// @file	QmlTestWindow.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   06.12.2019   18:19
///

#pragma once

#include <QMainWindow>

class QWidget;
class QQmlApplicationEngine;


namespace qml_gui
{
	class QmlTestWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit QmlTestWindow(QWidget* parent = nullptr);
		~QmlTestWindow();
	private:
		QQmlApplicationEngine* m_engine;
	};
}
