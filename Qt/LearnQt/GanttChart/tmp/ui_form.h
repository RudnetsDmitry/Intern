/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QCustomPlot *customPlot;
    QTableWidget *tableWidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *makeGraph;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *insertRow;
    QPushButton *removeRow;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(1680, 768);
        customPlot = new QCustomPlot(Form);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setGeometry(QRect(0, 0, 851, 790));
        tableWidget = new QTableWidget(Form);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(860, 10, 815, 621));
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(1180, 20, 241, 16));
        QFont font;
        font.setPointSize(9);
        label->setFont(font);
        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1480, 20, 171, 16));
        label_2->setFont(font);
        makeGraph = new QPushButton(Form);
        makeGraph->setObjectName(QString::fromUtf8("makeGraph"));
        makeGraph->setGeometry(QRect(860, 650, 131, 23));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1020, 650, 47, 13));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1020, 730, 47, 13));
        label_5 = new QLabel(Form);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1020, 690, 47, 13));
        label_6 = new QLabel(Form);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(1310, 650, 47, 13));
        insertRow = new QPushButton(Form);
        insertRow->setObjectName(QString::fromUtf8("insertRow"));
        insertRow->setGeometry(QRect(860, 690, 131, 21));
        removeRow = new QPushButton(Form);
        removeRow->setObjectName(QString::fromUtf8("removeRow"));
        removeRow->setGeometry(QRect(860, 730, 131, 21));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "\320\224\320\270\320\260\320\263\321\200\320\260\320\274\320\274\320\260 \320\223\320\260\320\275\321\202\320\260", nullptr));
        label->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        makeGraph->setText(QCoreApplication::translate("Form", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\264\320\270\320\260\320\263\321\200\320\260\320\274\320\274\321\203", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        label_5->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        insertRow->setText(QCoreApplication::translate("Form", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", nullptr));
        removeRow->setText(QCoreApplication::translate("Form", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
