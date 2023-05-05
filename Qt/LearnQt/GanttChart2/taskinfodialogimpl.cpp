//===========================================================================
//
//  Filename  : taskinfodialogimpl.cpp
//  Purpose   : Task Information Dialog (Subclass) Implementation
//  Copyright : Copyright (C) 2000 Matt J. Gumbley <matt@gumbley.demon.co.uk>
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
//#include "taskinfodialogimpl.h"
#include "task.h"

#include <qstring.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qdatetime.h>
//#include "calendar/calendar.h"

/*
 *  Constructs a TaskInfoDialogImpl which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TaskInfoDialogImpl::TaskInfoDialogImpl( QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl )
    : TaskInfoDialog( parent, name, modal=TRUE, fl )
{
	qDebug("Dialog constructor");
	view = GENERAL;
	task = 0;
}

/*
 *  Destroys the object and frees any allocated resources
 */
TaskInfoDialogImpl::~TaskInfoDialogImpl()
{
    // no need to delete child widgets, Qt does it all for us
	qDebug("Dialog destructor");
}

void TaskInfoDialogImpl::apply()
{
}

void TaskInfoDialogImpl::SetView( tidview v )
{
	view = v;
qDebug("taskinfodialogimpl::setview - finish me");
	// set the correct tab as current
	switch ( view )
	{
		case GENERAL:
			break;
		case PREDECESSORS:
			break;
		case NOTES:
			break;
		default:
			qWarning("TaskInfoDialogImpl::SetView unknown view");
	}
}

void TaskInfoDialogImpl::SetTask( Task * t )
{
	task = t;
qDebug("taskinfodialogimpl::settask - finish me");

	QString s;
	// Set up the dialog to reflect this task's information
	task->GetDescription( &s );
	TaskName->setText( s );

	TaskDuration->setValue( task->GetDuration() );

	int comboindex;
	switch( task->GetTaskType() )
	{
		case MAIN: comboindex = 1; break;
		case TASK: comboindex = 2; break;
		case MILE: comboindex = 0; break;
		default: qWarning("TaskInfoDialogImpl::SetTask unknown task type");
	}

	TaskType->setCurrentItem( comboindex );

	PercentComplete->setValue( task->GetCompleted() );

	QDate d;
	task->GetStartDate( &d );
	StartDateInput->setDate( d );

	task->GetExpEndDate( &d );
	ExpEndDateInput->setDate( d );

	task->GetEndDate( &d );
	EndDateInput->setDate( d );

	// TODO: PredecessorsListView, NotesEdit
}


