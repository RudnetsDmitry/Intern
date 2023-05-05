//===========================================================================
//
//  Filename  : main.cpp
//  Purpose   : QtGantt main routine
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

#include <stream.h>

#include <qapplication.h>
#include <qprinter.h>

#include "mainwidget.h"
#include "project.h"
#include "gantt.h" // for printing


int main( int argc, char **argv )
	{
	QApplication a( argc, argv );
	int i;
	char *filename = NULL;
	
	// There may be a "proper" C++/Qt way of handling the command line, but
	// I only know the C way, so here goes...
	for (i=1; i<argc; i++)
	{
		// qWarning("command line arg %d is [%s]\n", i, argv[i]);
		// Again, my C++ is not as good as my C... C++ string handling?
		// Handle options
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'p' : // print to this file
					if (i == (argc-1))
					{
						cerr << "Must specify an output file with -p\n";
						exit(-1);
					}
					char *output = argv[++i];
					Project *project = new Project( filename);
					Gantt *gantt = new Gantt( project, 0, "gantt" );
					QPrinter *printer = new QPrinter();
					gantt->printToFile( printer, output );
					exit(0);
					break;
			}
		}
		else
			filename = argv[i];
	}

	MainWidget w( a.argv()[1] );
	w.setGeometry( 0, 80, 790, 590 );
	w.setCaption( "QtGantt" );
	a.setMainWidget( &w );

// gives QLayout "unnamed" added to MainWidget "unnamed", which already had a
// layout.
//
	w.show();
	return a.exec();
	}
