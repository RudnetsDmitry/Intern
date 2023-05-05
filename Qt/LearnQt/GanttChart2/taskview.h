//===========================================================================
//
//  Filename  : taskview.h
//  Purpose   : task View Widget for QtGantt
//  Copyright : Copyright (C) 2000 Olivier Berrod <olivier.berrod@free.fr>
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


#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <qwidget.h>
#include "taskeditor.h"


class QDate;
class QString;
class QTableWidget;
class QListViewItem;

class QSplitter;
//class GanttChart;
class Project;

class TaskView : public QWidget
{
public:
	TaskView( Project *proj, QWidget* parent=0);
	~TaskView();
	void populate(bool force = false);
	void redraw();

public:
	void handleClick(int row, int column);

private:
	Project *project;
	QSplitter *splitter;
	QTableWidget * m_tableWidget;
	TaskField *detailview;

protected:
	void resizeEvent( QResizeEvent * );
	void paint( QPainter *painter );
private:
	void clearTable();
};

#endif // TASKVIEW_H

