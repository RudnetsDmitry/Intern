/****************************************************************************
** Form interface generated from reading ui file 'taskinfodialog.ui'
**
** Created: Mon Oct 16 22:24:49 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TASKINFODIALOG_H
#define TASKINFODIALOG_H

#include <qdialog.h>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class DateInput;
class QComboBox;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QMultiLineEdit;
class QPushButton;
class QSpinBox;
class QTabWidget;
class QWidget;

class TaskInfoDialog : public QDialog
{
public:
    TaskInfoDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE);
    ~TaskInfoDialog();

    QPushButton* buttonHelp;
    QPushButton* buttonApply;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QTabWidget* TabWidget;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QComboBox* TaskType;
    QSpinBox* PercentComplete;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLabel* TextLabel7;
    DateInput* StartDateInput;
    DateInput* ExpEndDateInput;
    DateInput* EndDateInput;
    QListView* PredecessorsListView;
    QMultiLineEdit* NotesEdit;
    QLabel* TextLabel1;
    QSpinBox* TaskDuration;
    QLabel* TextLabel2;
    QLineEdit* TaskName;

public:
    virtual void apply();

protected:
    QHBoxLayout* hbox;
};

#endif // TASKINFODIALOG_H
