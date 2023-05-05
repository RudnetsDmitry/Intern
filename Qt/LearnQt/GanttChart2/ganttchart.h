//===========================================================================
//
//  Filename  : ganttchart.h
//  Purpose   : Gantt Chart Widget for QtGantt
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


#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <qwidget.h>

class Project;

class GanttChart : public QWidget
{
public:
	GanttChart( Project *proj, QWidget* parent=0, const char* name=0 );
	~GanttChart();
	void setTaskHeight( int h );
	void setHeaderHeight( int h );
	void setStartTaskIndex( int t );
	int paintcount;

public:
	void changedaypix( int dpix );
	void setweekdetail( int newweekdetail );
	void draw();

private:
	int TEXTPIXY;			// number of pixels for the header (y)
	const static int FONTSIZE = 10;	// task font size
	int SPACING;			// vertical space between tasks
	int daypix;			// number of pixels between each day
	int weekdetail;			// whether weekends are filled in; each day shown
	int start_task;			// Starting index into the project task list (for scrolling)
	Project *project;

	void resizeProject();

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
};

#endif // GANTTCHART_H

