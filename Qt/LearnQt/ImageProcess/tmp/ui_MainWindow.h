/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenPicture;
    QAction *actionExit;
    QAction *actionSaveAs;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *pictureFrame;
    QVBoxLayout *verticalLayout_2;
    QLabel *pictureLabel;
    QHBoxLayout *filtersLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(977, 785);
        actionOpenPicture = new QAction(MainWindow);
        actionOpenPicture->setObjectName("actionOpenPicture");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName("actionSaveAs");
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        pictureFrame = new QFrame(centralWidget);
        pictureFrame->setObjectName("pictureFrame");
        pictureFrame->setFrameShape(QFrame::StyledPanel);
        pictureFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(pictureFrame);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        pictureLabel = new QLabel(pictureFrame);
        pictureLabel->setObjectName("pictureLabel");
        pictureLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(pictureLabel);


        verticalLayout->addWidget(pictureFrame);

        filtersLayout = new QHBoxLayout();
        filtersLayout->setSpacing(6);
        filtersLayout->setObjectName("filtersLayout");
        filtersLayout->setSizeConstraint(QLayout::SetMaximumSize);

        verticalLayout->addLayout(filtersLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 977, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpenPicture);
        menuFile->addAction(actionSaveAs);
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ch07-image-filter", nullptr));
        actionOpenPicture->setText(QCoreApplication::translate("MainWindow", "Open Picture...", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionSaveAs->setText(QCoreApplication::translate("MainWindow", "Save as...", nullptr));
        pictureLabel->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
