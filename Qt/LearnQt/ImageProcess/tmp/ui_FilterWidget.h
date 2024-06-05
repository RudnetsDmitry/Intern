/********************************************************************************
** Form generated from reading UI file 'FilterWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERWIDGET_H
#define UI_FILTERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *titleLabel;
    QLabel *thumbnailLabel;

    void setupUi(QWidget *FilterWidget)
    {
        if (FilterWidget->objectName().isEmpty())
            FilterWidget->setObjectName("FilterWidget");
        FilterWidget->resize(290, 312);
        verticalLayout = new QVBoxLayout(FilterWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(FilterWidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        titleLabel = new QLabel(frame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setMinimumSize(QSize(0, 25));
        titleLabel->setMaximumSize(QSize(16777215, 25));
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(titleLabel);

        thumbnailLabel = new QLabel(frame);
        thumbnailLabel->setObjectName("thumbnailLabel");
        thumbnailLabel->setMinimumSize(QSize(0, 0));
        thumbnailLabel->setMaximumSize(QSize(16777215, 16777215));
        thumbnailLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(thumbnailLabel);


        verticalLayout->addWidget(frame);


        retranslateUi(FilterWidget);

        QMetaObject::connectSlotsByName(FilterWidget);
    } // setupUi

    void retranslateUi(QWidget *FilterWidget)
    {
        FilterWidget->setWindowTitle(QCoreApplication::translate("FilterWidget", "Form", nullptr));
        titleLabel->setText(QCoreApplication::translate("FilterWidget", "Filter title", nullptr));
        thumbnailLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FilterWidget: public Ui_FilterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERWIDGET_H
