//===========================================================================
//
//  Filename  : task.cpp
//  Purpose   : Task Model for QtGantt
//  Copyright : Copyright (C) 2000 Matt J. Gumbley <matt@gumbley.demon.co.uk>
//  Original  : by Gunnstein Lye
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

#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>
#include <qstring.h>

#include "task.h"
#include "project.h"
#include "datehelper.h"

Task::Task( Project * owner )
{
	num = 0; // remove?
	type = TASK;
	this->owner = owner;

	// unused for now...
	constrained = collapsed = false;
	durationtype = DAYS;
	durationlength = 0;

	startdate = 0;
	enddate = 0;
	after = 0;
	completed = 0;
	description = 0;
}

Task::~Task()
{
	if (startdate) {
		delete startdate;
		startdate = 0;
	}
	if (enddate) {
		delete enddate;
		enddate = 0;
	}
	if (description) {
		delete description;
		description = 0;
	}
}

int Task::GetNumber(void)
{
	return num;
}


void Task::GetStartDate(QDate *date)
{
	if (startdate)
	{
		//debug(startdate->toString());
		*date = *startdate;
	}
}

void Task::SetStartDate(QDate *date)
{
	//    debug(startdate->toString());
	*startdate = *date ;
	owner->SetChanged();
}

void Task::SetStartDate(QString *local_string)
{
	//    debug(startdate->toString());

	QDate *local_date;
	local_date =new QDate();

	DateHelper::String2Date(local_string,local_date);
	this->SetStartDate(local_date);
	if (local_date)
		delete local_date;
	owner->SetChanged();
}

void Task::GetExpEndDate(QDate *date)
{
	if (expenddate)
		*date = *expenddate;
}

void Task::SetExpEndDate(QDate *date)
{
	*expenddate = *date;
	owner->SetChanged();
}

void Task::SetExpEndDate(QString *local_string)
{
	//    debug(startdate->toString());

	QDate *local_date;
	local_date =new QDate();

	DateHelper::String2Date(local_string,local_date);
	this->SetExpEndDate(local_date);
	if (local_date)
		delete local_date;
	owner->SetChanged();
}

void Task::GetEndDate(QDate *date)
{
	if (enddate)
		*date = *enddate;
}

void Task::SetEndDate(QDate *date)
{
	*enddate = *date;
	owner->SetChanged();
}

void Task::SetEndDate(QString *local_string)
{
	QDate *local_date;
	local_date =new QDate();

	DateHelper::String2Date(local_string,local_date);
	this->SetEndDate(local_date);

	if (local_date)
		delete local_date;
	owner->SetChanged();
}

tasktype Task::GetTaskType()
{
	//    local_type=type;
	return type;
}

void Task::GetDescription(QString *local_string)
{
	if (description)
		*local_string= *description;
}

void Task::SetDescription(QString *local_string)
{
	*description=*local_string;
	owner->SetChanged();
}

int Task::GetDuration(void)
{
	if (startdate && enddate)
	{
		return startdate->daysTo( (const QDate &) *enddate );
	}
	else
	{
		return 0;
	}
}

void Task::SetDuration( int days )
{
	*enddate = startdate->addDays( days );
}

int Task::GetCompleted( void )
{
	return completed;
}

void Task::SetCompleted( int done )
{
	completed = done;
}

void Task::write( QTextStream &t )
{
	QString sd = DateHelper::Date2String( *startdate );
	QString eed = DateHelper::Date2String( *expenddate );
	QString ed = DateHelper::Date2String( *enddate );
	QString typestr;
	switch (type)
	{
		case MAIN: typestr = "main"; break;
		case TASK: typestr = "task"; break;
		case MILE: typestr = "mile"; break;
		default: qWarning("Unknown type in Task::Write");
	}

	// Hopefully 3 characters for the ID will be enough! We're limited by
	// the array size, so if that goes over 999, we're dead. - why??
	t << num << ":" << typestr << ":";
	t << sd;
	t << ":";
	t << eed;
	t << ":";
	t << ed << ":" << after << ":" << completed << ":";
	t << *description;
	t << "\n";
// number:type:startdate:expenddate:enddate:after:complete:Task description
}

