/****************************************************************************
** Form implementation generated from reading ui file 'taskinfodialog.ui'
**
** Created: Mon Oct 16 22:24:49 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "stdafx.h"
#include "taskinfodialog.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qwidget.h>
//#include "calendar/calendar.h"
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a TaskInfoDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TaskInfoDialog::TaskInfoDialog( QWidget* parent,  const char* name, bool modal)
    : QDialog( parent)
{
    if ( !name )
	setName( "TaskInfoDialog" );
    resize( 564, 443 ); 
    setCaption( tr( "Task Information"  ) );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1 ) );

    QWidget* Layout1 = new QWidget( this, "Layout1" );
    Layout1->setGeometry( QRect( 10, 400, 538, 33 ) ); 
    hbox = new QHBoxLayout( Layout1 ); 
    hbox->setSpacing( 6 );
    hbox->setMargin( 0 );

    buttonHelp = new QPushButton( Layout1, "buttonHelp" );
    buttonHelp->setText( tr( "&Help"  ) );
    buttonHelp->setAutoDefault( TRUE );
    hbox->addWidget( buttonHelp );
    QSpacerItem* spacer = new QSpacerItem( 192, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hbox->addItem( spacer );

    buttonApply = new QPushButton( Layout1, "buttonApply" );
    buttonApply->setText( tr( "&Apply"  ) );
    buttonApply->setAutoDefault( TRUE );
    hbox->addWidget( buttonApply );

    buttonOk = new QPushButton( Layout1, "buttonOk" );
    buttonOk->setText( tr( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    hbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( Layout1, "buttonCancel" );
    buttonCancel->setText( tr( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    hbox->addWidget( buttonCancel );

    TabWidget = new QTabWidget( this, "TabWidget" );
    TabWidget->setGeometry( QRect( 11, 44, 538, 340 ) ); 

    QWidget* widget = new QWidget( TabWidget, "widget" );

    TextLabel3 = new QLabel( widget, "TextLabel3" );
    TextLabel3->setGeometry( QRect( 20, 20, 130, 20 ) ); 
    TextLabel3->setText( tr( "Percent complete:"  ) );

    TextLabel4 = new QLabel( widget, "TextLabel4" );
    TextLabel4->setGeometry( QRect( 270, 20, 50, 20 ) ); 
    TextLabel4->setText( tr( "Type:"  ) );

    TaskType = new QComboBox( FALSE, widget, "TaskType" );
    TaskType->insertItem( tr( "Milestone" ) );
    TaskType->insertItem( tr( "Main task" ) );
    TaskType->insertItem( tr( "Sub-task" ) );
    TaskType->setGeometry( QRect( 350, 20, 160, 22 ) ); 

    PercentComplete = new QSpinBox( widget, "PercentComplete" );
    PercentComplete->setGeometry( QRect( 160, 20, 60, 25 ) ); 

    TextLabel5 = new QLabel( widget, "TextLabel5" );
    TextLabel5->setGeometry( QRect( 20, 70, 121, 17 ) ); 
    TextLabel5->setText( tr( "Start:"  ) );

    TextLabel6 = new QLabel( widget, "TextLabel6" );
    TextLabel6->setGeometry( QRect( 19, 109, 121, 17 ) ); 
    TextLabel6->setText( tr( "Expected end:"  ) );

    TextLabel7 = new QLabel( widget, "TextLabel7" );
    TextLabel7->setGeometry( QRect( 20, 150, 121, 17 ) ); 
    TextLabel7->setText( tr( "Actual end:"  ) );

    StartDateInput = new DateInput( widget, "StartDateInput" );
    StartDateInput->setGeometry( QRect( 160, 70, 140, 30 ) ); 

    ExpEndDateInput = new DateInput( widget, "ExpEndDateInput" );
    ExpEndDateInput->setGeometry( QRect( 160, 110, 140, 30 ) ); 

    EndDateInput = new DateInput( widget, "EndDateInput" );
    EndDateInput->setGeometry( QRect( 160, 150, 140, 30 ) ); 
    TabWidget->insertTab( widget, tr( "General" ) );

    QWidget* widget_2 = new QWidget( TabWidget, "widget_2" );

    PredecessorsListView = new QListView( widget_2, "PredecessorsListView" );
    PredecessorsListView->addColumn( tr( "ID" ) );
    PredecessorsListView->header()->setClickEnabled( FALSE, PredecessorsListView->header()->count() - 1 );
    PredecessorsListView->addColumn( tr( "Task Name" ) );
    PredecessorsListView->header()->setClickEnabled( FALSE, PredecessorsListView->header()->count() - 1 );
    PredecessorsListView->addColumn( tr( "Type" ) );
    PredecessorsListView->header()->setClickEnabled( FALSE, PredecessorsListView->header()->count() - 1 );
    PredecessorsListView->setGeometry( QRect( 10, 10, 510, 290 ) ); 
    TabWidget->insertTab( widget_2, tr( "Predecessors" ) );

    QWidget* widget_3 = new QWidget( TabWidget, "widget_3" );

    NotesEdit = new QMultiLineEdit( widget_3, "NotesEdit" );
    NotesEdit->setGeometry( QRect( 6, 6, 521, 290 ) ); 
    TabWidget->insertTab( widget_3, tr( "Notes" ) );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 12, 12, 40, 17 ) ); 
    TextLabel1->setText( tr( "Name:"  ) );

    TaskDuration = new QSpinBox( this, "TaskDuration" );
    TaskDuration->setGeometry( QRect( 460, 10, 90, 25 ) ); 

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 390, 10, 57, 17 ) ); 
    TextLabel2->setText( tr( "Duration:"  ) );

    TaskName = new QLineEdit( this, "TaskName" );
    TaskName->setGeometry( QRect( 60, 10, 310, 22 ) ); 
    TaskName->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0 ) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonApply, SIGNAL( stateChanged(int) ), this, SLOT( apply() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TaskInfoDialog::~TaskInfoDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void TaskInfoDialog::apply()
{
    qWarning( "TaskInfoDialog::apply(): Not implemented yet!" );
}

