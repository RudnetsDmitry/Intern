//===========================================================================
//
//  Filename  : project.h
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

#ifndef PROJECT_H
#define PROJECT_H

class QString;
class QDate;
class QFile;
class QTextStream;

class Task;

class Project
{
	public:
		Project( QString const & filename );
		~Project();
		bool HasFilename();
		bool SetFilename( const QString & filename );
		void openFilename( const QString & filename );
		bool save( );

		const static int NUMTASKS = 100;  // max number of tasks to read
		// I will switch to using a QIntList (or some such expandable
		// container class) sometime soon.
		// Also note the number of characters allocated for the ID when
		// writing out the file, in Task::write
		Task *tasks[NUMTASKS];			// array of tasks
		// Please use GetNumTasks() instead of reading this, it'll all
		// be made private eventually.
		int numtasks;			// current number of tasks read

		// Please use GetStartDate() to return this:
		QDate *startdate;		// Project starts here
		QString *name;			// Project name is...

		void GetName(QString *local_string);
		void SetName(QString *local_string);

		int  GetNumTasks( );		// Return number of tasks in project
		void GetStartDate( QDate *date );
		void SetStartDate( QDate *date );

		void GetEndDate( QDate *date );	// Returns derived final date in 
						// project, determined by checking 
						// all tasks' end dates.
		bool isChanged( );		// Project changed?
		void SetChanged( );		// Yes, it has
		void reset( );			// Re-Initialise
		void refresh( );		// Re-load

	private:
		QString m_filename;
		// we're not using the realloc() stuff at the moment
		int numallocated;	// current size of tasks array
		bool changed;		// Has the project data changed at all?

		void addtasks();	// Reallocate tasks array
		void openQFile( QFile & file );
		bool saveQFile( QFile & file );
		void read( QTextStream *t );	// Read project stream
		void write( QTextStream &t );	// Write project stream
};


#endif // PROJECT_H

