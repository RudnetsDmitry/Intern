//===========================================================================
//
//  Filename  : taskeditor.cpp
//  Purpose   : Task Editor Widget for QtGantt
//  Copyright : Copyright (C) 2000 Olivier Berrod <obe@alcove.fr>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//===========================================================================
#include "stdafx.h"
#include <stdio.h>
#include <qlistview.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qsplitter.h>
#include <qstring.h>

#include <qdatetime.h> 
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "task.h"
#include "project.h"
#include "taskeditor.h"


TaskField::TaskField( QWidget *parent)
        : QWidget( parent)
{

	// Make the top-level layout; a vertical box to contain all widgets and
	// sub-layouts.  QBoxLayout *topLayout = new QVBoxLayout( this, 5 ); 

	// Let the grid-layout have a spacing of 10 pixels between widgets,
	// overriding the default from topLayout.
	//QGridLayout *grid = new QGridLayout( numRows, 3, 10 );
	//topLayout->addLayout( grid );

	resize( 679, 360 );
	setWindowTitle(tr("Form1"));

	QWidget* MainLayout1 = new QWidget( this);
	MainLayout1->setGeometry( QRect( 20, 20, 360, 160 ) );    
	//MainLayout1->setBackgroundColor( "white" );
	layout1 = new QVBoxLayout( MainLayout1 ); 

	LineEdit_description = new QLineEdit( MainLayout1);
	LineEdit_description->setFixedHeight(LineEdit_description->sizeHint().
			height());
	layout1->addWidget( LineEdit_description );
	connect(LineEdit_description, SIGNAL(textChanged(const QString&)),
			this, SLOT(change_description(const QString&)));

	LineEdit_start_date = new QLineEdit( MainLayout1);
	LineEdit_start_date->setFixedHeight(LineEdit_start_date->sizeHint().
			height());
	layout1->addWidget( LineEdit_start_date );
	connect(LineEdit_start_date, SIGNAL(textChanged(const QString&)),
			this, SLOT(change_startdate(const QString&)));

	LineEdit_stop_date = new QLineEdit( MainLayout1);
	LineEdit_stop_date->setFixedHeight( LineEdit_stop_date->sizeHint().
			height());
	layout1->addWidget( LineEdit_stop_date );
	connect(LineEdit_stop_date, SIGNAL(textChanged(const QString&)),
			this, SLOT(change_enddate(const QString&)));
	//    layout1->addStretch( 1 );

	ComboBox_type = new QComboBox(MainLayout1);
	// Why add a blank element?
	ComboBox_type->addItem( tr( "" ) );
	ComboBox_type->addItem( tr( "Main" ) );
	ComboBox_type->addItem( tr( "Task" ) );
	ComboBox_type->addItem( tr( "Mile" ) );
	ComboBox_type->setFixedHeight(ComboBox_type->sizeHint().height());
	//ComboBox_type->setBackgroundColor( "white" );
	layout1->addWidget( ComboBox_type );


	QWidget* Layout11 = new QWidget( this);
	//    Layout11->setGeometry( QRect( 510, 20, 160, 100 ) ); 
	Layout11->setGeometry( QRect( 20, 200, 160, 100 ) ); 
	//Layout11->setBackgroundColor( "white" );
	layout2 = new QVBoxLayout( Layout11 ); 
	layout2->setSpacing( 6 );
	//layout2->setMargin( 0 );

	LineEdit10 = new QLineEdit( Layout11);
	LineEdit10->setFixedHeight(LineEdit10->sizeHint().height());
	layout2->addWidget( LineEdit10 );

	LineEdit11 = new QLineEdit( Layout11);
	LineEdit11->setFixedHeight(LineEdit11->sizeHint().height());
	layout2->addWidget( LineEdit11 );

	SpinBox2 = new QSpinBox( Layout11);    
	SpinBox2->setFixedHeight(SpinBox2->sizeHint().height());
	layout2->addWidget( SpinBox2 );

	PushButton_update = new QPushButton( this);
	PushButton_update->setGeometry( QRect( 190, 250, 104, 28 ) ); 
	PushButton_update->setText( tr( "Update Task"  ) );
}

void TaskField::RefreshValues()
{
	if (!LocalTask)
	{
		LineEdit_description->setText("Pointeur Null");
		return;
	}

	QString  task_text;
	QDate    date;
	LocalTask->GetDescription(&task_text);
	LineEdit_description->setText(task_text);

	// Print the start date
	LocalTask->GetStartDate(&date);
	LineEdit_start_date->setText(QString("%1 / %2 / %3")
		.arg(date.day()).arg(date.month()).arg(date.year()));

	// Print the end date
	LocalTask->GetEndDate(&date);
	LineEdit_stop_date->setText(QString("%1 / %2 / %3")
		.arg(date.day()).arg(date.month()).arg(date.year()));
	this->RefreshComBox_type();
}

void TaskField::RefreshComBox_type()
{
	tasktype type;

	type = LocalTask->GetTaskType();
	ComboBox_type->setCurrentIndex(static_cast<int>(type)+1) ;
}
        
void TaskField::SetTask(Task  *task)
{
    LocalTask = task;
}




// Public slot:

void TaskField::change_description(const QString& item)
{
	QString *test;

	test =new QString;

	*test=(QString)item;

	//debug("description change\n");
	LocalTask->SetDescription(test);

	delete test;
}



//void TaskField::change_startdate()
void TaskField::change_startdate(const QString& item)
{
	//debug("start date change\n");

	//    LocalTask->SetStartDate(&LineEdit_start_date->text());

	QString *tempo;
	tempo =new QString(item);

	LocalTask->SetStartDate(tempo);
	if (tempo) 
		delete tempo;
}

void TaskField::change_enddate(const QString& item)
{
	QString *tempo;
	tempo =new QString(item);

	LocalTask->SetEndDate(tempo);
	if (tempo)
	       delete tempo;
}



// Private

void TaskField::paintEvent( QPaintEvent * )
{
}


