//===========================================================================
//
//  Filename  : gantt.h
//  Purpose   : Gantt View for QtGantt
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


#ifndef GANTT_H
#define GANTT_H

#include <qwidget.h>
#include "project.h"

class QPainter;
class QPrinter;
class QPrintDialog;
class QFile;
class QFileDialog;
class QTextStream;
class QString;
class QDate;


class Gantt : public QWidget
	{
	public:
		Gantt( Project *proj, QWidget *parent=0, const char *name=0 );

		const static int NUMTASKS = 100;  // max number of tasks to read
		const static int TITLEPIXY = 30;   // number of pixels for the title (y)
		int TEXTPIXY;   // number of pixels for the header (y)
		const static int FONTSIZE = 10;   // task font size
		const static int SPACING = 25;    // vertical space between tasks
		int CALHEIGHT;                    // number of pixels for the calendar info (y)
		int textpixx;          // number of pixels for the description (x)
		int daypix;            // number of pixels between each day
		int weekdetail;        // whether weekends are filled in; each day shown
		int isprinting;
		int paintcount;
	public:
		void print( QPrinter *printer );
		void printToFile( QPrinter *printer, char * filename );
		void draw();
		void changetextpix( int tpix );
		void changedaypix( int dpix );
		void setweekdetail( int newweekdetail );
	protected:
		void paint( QPainter *painter );
		void paintEvent( QPaintEvent *e );
		void painttask( QPainter *painter, int taskno, int y );
		void paintmain( QPainter *painter, int taskno, int y );
		void paintmile( QPainter *painter, int taskno, int y );
		void painttaskbeginarrow( QPainter *painter, int taskno, int y, int j );
		void painttaskendarrow( QPainter *painter, int taskno, int y, int j );
		void paintmainbeginarrow( QPainter *painter, int taskno, int y, int j );
		void paintmainendarrow( QPainter *painter, int taskno, int y, int j );
		void paintmilebeginarrow( QPainter *painter, int taskno, int y, int j );
		void paintmileendarrow( QPainter *painter, int taskno, int y, int j );
	private:
		Project *project;
		QPrinter *printer; // Only set when printing
	};

#endif // GANTT_H
