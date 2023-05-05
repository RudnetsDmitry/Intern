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
#include <QBoxLayout>
#include <QFrame>

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
		m_engine->load(QUrl(QStringLiteral("qrc:/main1.qml")));
		auto const& rootObjects = m_engine->rootObjects();
		if (rootObjects.isEmpty())
			return;

		auto qmlWidget = qobject_cast<QWindow*>(rootObjects.at(0));
		QWidget* container = QWidget::createWindowContainer(qmlWidget);
		container->setMinimumSize(qmlWidget->size().width(), qmlWidget->size().height() + 30);

		QFrame * centerWidget = new QFrame();
		centerWidget->setFrameShape(QFrame::WinPanel);
		QVBoxLayout* vbox = new QVBoxLayout();
		vbox->setContentsMargins(5, 5, 5, 5);

		QFrame* innerWidget = new QFrame();
		innerWidget->setFrameShape(QFrame::Panel);

		QVBoxLayout* innervbox = new QVBoxLayout();
		innervbox->setContentsMargins(20, 40, 20, 20);
		innervbox->addWidget(container);
		innerWidget->setLayout(innervbox);

		vbox->addWidget(innerWidget);
		centerWidget->setLayout(vbox);
		
		setCentralWidget(centerWidget);

		QMetaObject::connectSlotsByName(this);
	}

	QMainWindow* MakeQmlWindow()
	{
		return new QmlTestWindow();
	}

	

}
