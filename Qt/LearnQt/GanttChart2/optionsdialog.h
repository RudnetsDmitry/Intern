/****************************************************************************
** Form interface generated from reading ui file 'optionsdialog.ui'
**
** Created: Sat Oct 7 21:59:37 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;
class QPushButton;

class OptionsDialog : public QDialog
{
public:
    OptionsDialog( QWidget* parent = 0);
    ~OptionsDialog();

    QPushButton* buttonHelp;
    QPushButton* buttonApply;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLabel* TextLabel1;
    QComboBox* DateFormat;

protected:
    QHBoxLayout* hbox;
};

#endif // OPTIONSDIALOG_H
