//===========================================================================
//
//  Filename  : task.h
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

#ifndef TASK_H
#define TASK_H

#include <qtextstream.h>

class QDate;
class QString;
class Project;

enum tasktype { MAIN, TASK, MILE };
enum timetype { HOURS, DAYS, WEEKS };


class Task
{
	// Oh, how horrific, it's exposing itself in public.
	// (We'll make most of this private with accessors later)
	public:
		Task( Project *p );
		~Task();

		int GetNumber(void);
		void GetStartDate(QDate *date);
		void SetStartDate(QDate *date);
		void SetStartDate(QString *local_string);

		void GetExpEndDate(QDate *date);
		void SetExpEndDate(QDate *date);
		void SetExpEndDate(QString *local_string);

		void GetEndDate(QDate *date);
		void SetEndDate(QDate *date);
		void SetEndDate(QString *local_string);

		tasktype GetTaskType(void);
		void GetDescription(QString *local_string);
		void SetDescription(QString *local_string);

		// In Days...
		int GetDuration(void);
		void SetDuration(int);

		int GetCompleted(void);
		void SetCompleted(int);

		void write( QTextStream &t );	// Write task to stream

		int num; // do we really need this? it's an array!
		tasktype type;

		// this next bit is not yet used....
		// A task can either have a duration or a start/end date
		// but not both (?)
		bool constrained;
		bool collapsed;		// For MAIN tasks, is the
					// outline collapsed (so 
					// all subtasks up to the 
					// next MAIN are hidden)?
		timetype durationtype;
		int durationlength;
		// but this bit is used...
	
		QDate   *startdate;
		QDate   *enddate;
		QDate   *expenddate;
		int after;	// need multiple predecessors
		int completed;
		QString *description;

private:
		// Set the task Description
		int task_follow;
		int task_need;
		// so that changes to individual tasks can set the project's
		// changed flag
		Project *owner; 
};


#endif // TASK_H
