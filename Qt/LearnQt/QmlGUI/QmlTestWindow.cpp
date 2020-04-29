/// @file	QmlTestWindow.cpp
///
/// @see   QmlTestWindow.h
///
/// @author	RDN © Intern :)
///
/// @date   06.12.2019   18:22
///


#include "StdAfx.h"
#include "QmlTestWindow.h"
#include "ExportFunctions.h"

#include <QQuickWidget>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <limits>
#include <QApplication>


namespace qml_gui
{
	QmlTestWindow::~QmlTestWindow()
	{
		delete m_engine;
	}

	QmlTestWindow::QmlTestWindow(QWidget* parent /*= nullptr*/)
		: m_engine(new QQmlApplicationEngine())
	{
		setObjectName(QString::fromLatin1("qml_gui::QmlTestWindow"));
		resize(927, 644);

		//m_engine->addImportPath(QStringLiteral("qrc:/"));
		m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
		auto const& rootObjects = m_engine->rootObjects();
		if (rootObjects.isEmpty())
			return;

		auto qmlWidget = qobject_cast<QWindow*>(rootObjects.at(0));
		QWidget* container = QWidget::createWindowContainer(qmlWidget, this);
		
		setCentralWidget(container);
		
		QMetaObject::connectSlotsByName(this);
	}

	QMainWindow* MakeQmlWindow()
	{
		return new QmlTestWindow();
	}

}
