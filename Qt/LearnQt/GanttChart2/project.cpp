//===========================================================================
//
//  Filename  : project.cpp
//  Purpose   : Project Model for QtGantt
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
//#include <iostream.h>

#include <qfile.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qtextstream.h>

#include "task.h"
#include "project.h"
#include "datehelper.h"

Project::Project(QString const & filename)
{
	int t;
	for (t = 0; t < NUMTASKS; t++)
	{
		tasks[t] = 0;
	}
	name = 0;
	startdate = 0;
	numtasks = 0;
	changed = false;
	if (!filename.isEmpty())
		openFilename(filename);
}

Project::~Project()
{
	reset();
	// any more deallocation to be done here?
}

bool Project::HasFilename()
{
	return !m_filename.isEmpty();
}

bool Project::SetFilename(const QString & filename)
{
	if (filename.isNull())
	{
		qWarning("Project::SetFilename - Null filename");
		return false;
	}
	else
		m_filename = filename;
	return true;
}

// must set this->filename if OK
void Project::openFilename(const QString & filename)
{
	reset();
	m_filename = filename;
	QFile file(filename);
	if (file.exists())
		openQFile(file);
	else
	{
		qWarning("file %s doesn't exist\n", filename);
		m_filename.clear();
	}
}

bool Project::save()
{
	qDebug("Project::save()");
	// Filename should already have been set
	if (m_filename.isEmpty())
	{
		qWarning("Project::save - BUG - filename has not been set");
		return false;
	}
	QFile file(m_filename);
	bool retval = saveQFile(file);
	if (retval)
	{
		qDebug("Project::saveQFile OK");
		changed = false;
	}
	else
	{
		qDebug("Project::saveQFile Failed");
	}
	return retval;
}

void Project::SetChanged()
{
	changed = true;
	// notify anything?
}

void Project::refresh()
{
	if (m_filename.isEmpty())
		return;

	QString oldfilename = m_filename;
	reset();
	m_filename = oldfilename;
	QFile file(oldfilename);
	if (file.exists())
		openQFile(file);
	else
	{
		m_filename.clear();
	}
}

void Project::GetName(QString * local_string)
{
	if (name)
		*local_string = *name;
}

void Project::SetName(QString * local_string)
{
	*name = *local_string;
	changed = true;
}

int Project::GetNumTasks()
{
	return numtasks;
}

void Project::GetStartDate(QDate * date)
{
	if (startdate)
		*date = *startdate;
}

void Project::SetStartDate(QDate * date)
{
	*startdate = *date;
	changed = true;
}


void Project::GetEndDate(QDate * date)
{
	Task * t;
	QDate * taskdate = new QDate();
	// Start at the epoch, and find the last date in the project
	if (!(date->setDate(1970, 1, 1)))
		qWarning("project::GetEndDate couldn't set date to epoch");
	for (int i = 0; i < numtasks; i++)
	{
		t = tasks[i];
		t->GetEndDate(taskdate);
		if (*taskdate > *date)
			*date = *taskdate;
		t->GetExpEndDate(taskdate);
		if (*taskdate > *date)
			*date = *taskdate;
	}
}

bool Project::isChanged()
{
	return changed;
}

void Project::reset()
{
	int t;
	for (t = 0; t < numtasks; t++)
	{
		if (tasks[t])
		{
			delete tasks[t];
			tasks[t] = 0;
		}
	}
	if (name)
	{
		delete name;
		name = 0;
	}
	if (startdate)
	{
		delete startdate;
		startdate = 0;
	}
	// delete tasks;
	numtasks = 0;
	// not so sure whether changed should be true or false?
	changed = false;
	m_filename.clear();
}

// private
void Project::openQFile(QFile & file)
{
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream t(&file);
		read(&t);
		file.close();
	}
	else
	{
		qWarning("Project::openQFile - Could not open file");
	}
}

// private
bool Project::saveQFile(QFile & file)
{
	qDebug("Project::saveQFile( QFile * )");
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream t(&file);
		// How can I know if t is OK or not?
		qDebug("writing to stream");
		write(t);
		qDebug("closing stream");
		file.close();
		return true;
	}
	else
	{
		qWarning("Project::saveQFile - Could not create file");
	}
	return false;
}

void Project::read(QTextStream * t)
{
	QString qstr1, qstr2, yyyy, mm, dd;
	numtasks = 0;
	int pos1, pos2, pos3;

	while (!t->atEnd() && numtasks < NUMTASKS)     // loops through the file
	{
		qstr1 = t->readLine();

		if (!qstr1.isEmpty() && qstr1.left(1) != "#")  // if not an empty line and not a comment
		{
			if (qstr1.left(1) == "%")    // if a header line
			{
				pos1 = qstr1.indexOf('%', 1);
				qstr2 = qstr1.mid(1, pos1 - 1);

				pos2 = qstr2.indexOf('-', 0);
				dd = qstr2.left(pos2);
				pos3 = qstr2.indexOf('-', pos2 + 1);
				mm = qstr2.mid(pos2 + 1, pos3 - pos2 - 1);
				yyyy = qstr2.right(qstr2.length() - pos3 - 1);
				startdate = new QDate(yyyy.toInt(), mm.toInt(), dd.toInt());

				qstr2 = qstr1.right(qstr1.length() - pos1 - 1);
				name = new QString(qstr2);
			}
			else                   // if a task line
			{
				tasks[numtasks] = new Task(this);
				// num
				pos1 = qstr1.indexOf(':', 0);
				qstr2 = qstr1.left(pos1);
				tasks[numtasks]->num = qstr2.toInt();

				// type
				pos2 = qstr1.indexOf(':', pos1 + 1);
				qstr2 = qstr1.mid(pos1 + 1, pos2 - pos1 - 1);
				if (qstr2 == "main")
					tasks[numtasks]->type = MAIN;
				else if (qstr2 == "task")
					tasks[numtasks]->type = TASK;
				else if (qstr2 == "mile")
					tasks[numtasks]->type = MILE;
				else
				{
					qWarning("Unknown task type '%s'\n", &qstr2);
				}

				// startdate
				pos1 = qstr1.indexOf(':', pos2 + 1);
				qstr2 = qstr1.mid(pos2 + 1, pos1 - pos2 - 1);
				pos3 = qstr2.indexOf('-', 0);
				dd = qstr2.left(pos3);
				pos2 = qstr2.indexOf('-', pos3 + 1);
				mm = qstr2.mid(pos3 + 1, pos2 - pos3 - 1);
				yyyy = qstr2.right(qstr2.length() - pos2 - 1);
				tasks[numtasks]->startdate = new QDate(yyyy.toInt(), mm.toInt(), dd.toInt());

				// expenddate
				pos2 = qstr1.indexOf(':', pos1 + 1);
				if (tasks[numtasks]->type != MILE)
					// if not a milestone
				{
					qstr2 = qstr1.mid(pos1 + 1, pos2 - pos1 - 1);
					pos3 = qstr2.indexOf('-', 0);
					dd = qstr2.left(pos3);
					pos1 = qstr2.indexOf('-', pos3 + 1);
					mm = qstr2.mid(pos3 + 1, pos1 - pos3 - 1);
					yyyy = qstr2.right(qstr2.length() - pos1 - 1);
					tasks[numtasks]->expenddate = new QDate(yyyy.toInt(), mm.toInt(), dd.toInt());
				}
				else         // milestones don't need enddate, so a blank entry is ok
					tasks[numtasks]->expenddate = new QDate(tasks[numtasks]->startdate->addDays(0));


				// enddate
				pos1 = pos2;
				pos2 = qstr1.indexOf(':', pos1 + 1);
				if (tasks[numtasks]->type != MILE)
					// if not a milestone
				{
					qstr2 = qstr1.mid(pos1 + 1, pos2 - pos1 - 1);
					pos3 = qstr2.indexOf('-', 0);
					if (pos3 != -1)
					{
						dd = qstr2.left(pos3);
						pos1 = qstr2.indexOf('-', pos3 + 1);
						mm = qstr2.mid(pos3 + 1, pos1 - pos3 - 1);
						yyyy = qstr2.right(qstr2.length() - pos1 - 1);
						tasks[numtasks]->enddate = new QDate(yyyy.toInt(), mm.toInt(), dd.toInt());
					}
					else
					{
						tasks[numtasks]->enddate = new QDate(tasks[numtasks]->expenddate->addDays(0));
					}
				}
				else         // milestones don't need enddate, so a blank entry is ok
					tasks[numtasks]->enddate = new QDate(tasks[numtasks]->startdate->addDays(0));

				// after
				pos1 = qstr1.indexOf(':', pos2 + 1);
				qstr2 = qstr1.mid(pos2 + 1, pos1 - pos2 - 1);
				tasks[numtasks]->after = qstr2.toInt();

				// percentage copleted
				pos2 = qstr1.indexOf(':', pos1 + 1);
				qstr2 = qstr1.mid(pos1 + 1, pos2 - pos1 - 1);
				tasks[numtasks]->completed = qstr2.toInt();

				// description
				qstr2 = qstr1.right(qstr1.length() - pos2 - 1);
				tasks[numtasks]->description = new QString(qstr2);

				numtasks++;
			}
		}
	}
}

void Project::write(QTextStream & t)
{
	QString qstr1, qstr2, yyyy, mm, dd;
	int pos1, pos2, pos3;

	qDebug("Project::write");
	t << "# Header line:\n";
	t << "# %Project start date%Project description\n";
	t << "%";
	QString sd = DateHelper::Date2String(*startdate);
	t << sd;
	t << "%";
	t << *name;
	t << "\n\n";
	t << "# Task lines:\n";
	t << "# number:type:startdate:expenddate:enddate:after:complete:Task description\n\n";
	qDebug("Writing tasks...");
	for (int i = 0; i < numtasks; i++)
	{
		tasks[i]->write(t);
	}
	t << "\n";
	qDebug("done...");
	/*

# Header line:
# %Project start date%Project description
%10-10-2000%GanttChart Task - Main arrows are Just Plain Wrong

# Task lines:
# number:type:startdate:expenddate:enddate:after:complete:Task description

 1:task:20-10-2000:21-10-2000:21-10-2000:0:0:After Matrix Task - Main

	*/
}

