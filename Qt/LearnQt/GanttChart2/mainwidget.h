//===========================================================================
//
//  Filename  : mainwidget.h
//  Purpose   : Main Application Controller Widget for QtGantt
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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <qmainwindow.h>

#include "gantt.h"
#include "ganttview.h"
#include "calendarview.h"
#include "taskview.h"
#include "project.h"

#define MINDAYPIX 1
#define MAXDAYPIX 40

class QVBox;
class QTabWidget;
class QScrollBar;
class QSpinBox;
class QShortcut;
class QMenuBar;
class QTabBar;
class QStatusBar;
class QPopupMenu;

class MainWidget : public QMainWindow
	{
		// because MainWidget contains slots
	public:
		MainWidget( QString const & filenamearg, QWidget *parent=0, const char *name=0 );
	protected:
		void resizeEvent( QResizeEvent * );
	private:
		QTabWidget *tabwidget;
		Project *project;		// This is the model

		//Gantt *gantt;			// This is the old view
		GanttView *ganttview;		// New Gantt View
		CalendarView *calendarview;	// Calendar View
		TaskView *taskview;		// Task Editor View

		// These should be in Gantt
		QScrollBar *scrolltextpix;
		QSpinBox *spindaypix;

		QShortcut *f5acc;
		QShortcut *plusacc;
		QShortcut *minusacc;
		QMenuBar *_menubar;
		QAction * previewViewID;
		QAction * ganttViewID;
		QAction * taskViewID;
		QAction * calViewID;
		QStatusBar *_statusbar;
		QMenu *_filemenu;
		QMenu *_viewmenu;
		QMenu *_options;
		QMenu *_helpmenu;
		// Move into Gantt/GanttView?
		QAction * weekdetailID;
		bool isWeekdetail;

		int frameT, frameY; // unimportant positioning stuff
		// frameT is the thickness of the frame; frameY is where it is
		// positioned vertically

	private:
		void slotNewFile();
		void slotOpenFile();
		void slotSaveFile();
		void slotRefreshFile();
		void slotPropertiesFile();
		void slotPrintFile();

		void slotViewTabSelected(int i);
		void slotPreviewView();
		void slotGanttView();
		void slotTaskView();
		void slotCalendarView();

		void slotLinkTasks();
		void slotUnlinkTasks();
		void slotInfoTask();
		void slotNoteTask();
		void slotAfterTask();
		void slotGotoTask();

		// Move into Gantt
		void slotchangetextpix(int tpix);
		void slotchangedaypix(int dpix);

		void slotQuit();
		void slotAbout();
		void slottoggleweekdetail();
		void slotzoomIn();
		void slotzoomOut();
	};

#endif // MAINWIDGET_H
