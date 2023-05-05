/****************************************************************************
** Form implementation generated from reading ui file 'projectinfodialog.ui'
**
** Created: Mon Oct 16 22:24:57 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "stdafx.h"
#include "projectinfodialog.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
//#include "calendar/calendar.h"
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProjectInfoDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectInfoDialog::ProjectInfoDialog( QWidget* parent)
    : QDialog( parent)
{
    resize( 519, 290 ); 
    setWindowTitle( tr( "Project Information"  ) );
    setSizeGripEnabled( TRUE );

    TextLabel2 = new QLabel( this);
    TextLabel2->setGeometry( QRect( 20, 60, 71, 20 ) ); 
    TextLabel2->setText( tr( "Start date:"  ) );

    //StartDate = new DateInput( this);
    //StartDate->setGeometry( QRect( 120, 60, 140, 30 ) ); 

    Title = new QLineEdit( this);
    Title->setGeometry( QRect( 120, 20, 380, 22 ) ); 
    Title->setSizePolicy( QSizePolicy( QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed) );

    TextLabel1 = new QLabel( this);
    TextLabel1->setGeometry( QRect( 20, 20, 30, 17 ) ); 
    TextLabel1->setText( tr( "Title:"  ) );

    QWidget* Layout1 = new QWidget( this);
    Layout1->setGeometry( QRect( 20, 240, 476, 33 ) ); 
    hbox = new QHBoxLayout( Layout1 ); 
    hbox->setSpacing( 6 );
    //hbox->setMargin( 0 );

    buttonHelp = new QPushButton( Layout1);
    buttonHelp->setText( tr( "&Help"  ) );
    buttonHelp->setAutoDefault( TRUE );
    hbox->addWidget( buttonHelp );
    QSpacerItem* spacer = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hbox->addItem( spacer );

    buttonApply = new QPushButton( Layout1);
    buttonApply->setText( tr( "&Apply"  ) );
    buttonApply->setAutoDefault( TRUE );
    hbox->addWidget( buttonApply );

    buttonOk = new QPushButton( Layout1);
    buttonOk->setText( tr( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    hbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( Layout1);
    buttonCancel->setText( tr( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    hbox->addWidget( buttonCancel );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonApply, SIGNAL( clicked() ), this, SLOT( apply() ) );

    // tab order
    setTabOrder( Title, buttonHelp );
    setTabOrder( buttonHelp, buttonApply );
    setTabOrder( buttonApply, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectInfoDialog::~ProjectInfoDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void ProjectInfoDialog::apply()
{
    qWarning( "ProjectInfoDialog::apply(): Not implemented yet!" );
}

