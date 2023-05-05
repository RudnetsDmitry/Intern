/****************************************************************************
** Form implementation generated from reading ui file 'optionsdialog.ui'
**
** Created: Sat Oct 7 21:59:45 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "stdafx.h"
#include "optionsdialog.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a OptionsDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OptionsDialog::OptionsDialog( QWidget* parent)
    : QDialog( parent)
{
    resize( 519, 288 );
    setWindowTitle(tr("Options"));
    setSizeGripEnabled( TRUE );

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

    TextLabel1 = new QLabel( this);
    TextLabel1->setGeometry( QRect( 30, 30, 79, 17 ) ); 
    TextLabel1->setText( tr( "Date Format"  ) );

    DateFormat = new QComboBox(this);
    DateFormat->addItem( tr( "dd-mm-yyyy" ) );
    DateFormat->addItem( tr( "mm-dd-yyyy" ) );
    DateFormat->addItem( tr( "yyyy-mm-dd" ) );
    DateFormat->setGeometry( QRect( 140, 30, 120, 22 ) ); 

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
OptionsDialog::~OptionsDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

