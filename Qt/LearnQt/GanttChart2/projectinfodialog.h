/****************************************************************************
** Form interface generated from reading ui file 'projectinfodialog.ui'
**
** Created: Mon Oct 16 22:24:57 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTINFODIALOG_H
#define PROJECTINFODIALOG_H

#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class DateInput;
class QLabel;
class QLineEdit;
class QPushButton;

class ProjectInfoDialog : public QDialog
{
public:
    ProjectInfoDialog( QWidget* parent = 0);
    ~ProjectInfoDialog();

    QLabel* TextLabel2;
    //DateInput* StartDate;
    QLineEdit* Title;
    QLabel* TextLabel1;
    QPushButton* buttonHelp;
    QPushButton* buttonApply;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public:
    virtual void apply();

protected:
    QHBoxLayout* hbox;
};

#endif // PROJECTINFODIALOG_H
