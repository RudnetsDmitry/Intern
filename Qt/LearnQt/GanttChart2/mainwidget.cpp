//===========================================================================
//
//  Filename  : mainwidget.cpp
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

#include "stdafx.h"
//#include <stream.h>

#include <qapplication.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpixmap.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qtabbar.h>
//#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qframe.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <qspinbox.h>
#include <qfile.h>
#include <qlayout.h>

// The MainWidget contains these view-widgets, layered on a QWidgetStack.
//#include "gantt.h" // the "print preview" Gantt table and chart in one
#include "ganttview.h" // Gantt Table, Splitter, and Gantt Chart
#include "taskview.h" // Task Editor View
#include "calendarview.h" // Calendar View

// The dialogs
//#include "taskinfodialogimpl.h"
#include "projectinfodialogimpl.h"

#include "mainwidget.h"
#include "project.h"

#include "pixmaps/newfile.xpm"
#include "pixmaps/openfile.xpm"
#include "pixmaps/savefile.xpm"
#include "pixmaps/printfile.xpm"
#include "pixmaps/linktasks.xpm"
#include "pixmaps/unlinktasks.xpm"
#include "pixmaps/infotask.xpm"
#include "pixmaps/notetask.xpm"
#include "pixmaps/aftertask.xpm"
#include "pixmaps/gototask.xpm"

#include <QtGui/QShortcut>

MainWidget::MainWidget(QString const & filenamearg, QWidget *parent, const char *name )
        : QMainWindow( parent/*, name */)
	{
	int ctrlH = 25; // might be better the same height as the menu?
	frameT = 5; // thickness between window edge and frame edge

	//
	// Create the Model
	//
	project = new Project( filenamearg );

	//
	// Create the menus
	//
	_menubar = menuBar(); // new QMenuBar( this );
	_menubar->addSeparator();
	QRect menuRect = _menubar->geometry();
	frameY = menuRect.height() + ctrlH + (2 * frameT);

	//
	// Populate the menus
	//
	_filemenu = _menubar->addMenu("&File");
	// All the menu slots are part of the MainWidget, which is the
	// controller. Project is the model; Gantt is one view.
	_filemenu->addAction("&New", this, [this]()
	{
		slotNewFile();
	});
	_filemenu->addAction( "&Open...", this, [this]()
	{
		slotOpenFile();
	});

	_filemenu->addAction("&Refresh", this, [this]()
	{
		slotRefreshFile();
	});

	_filemenu->addAction("&Save", this, [this]()
	{
		slotSaveFile();
	});

	_filemenu->addSeparator();
	_filemenu->addAction("Prop&erties", this, [this]()
	{
		slotPropertiesFile();
	});
	_filemenu->addSeparator();
	_filemenu->addAction("&Print", this, [this]()
	{
		slotPrintFile();
	});
	_filemenu->addSeparator();
	_filemenu->addAction("&Quit", this, [this]()
	{
		slotQuit();
	});

	_options = new QMenu;
	weekdetailID = _options->addAction("&Week detail", this, [this]()
	{
		slottoggleweekdetail();
	});
	isWeekdetail = false;
	weekdetailID->setCheckable(true);

	_viewmenu = _menubar->addMenu("&View");
	_viewmenu->addAction("Zoom in", this, [this]()
	{
		slotzoomIn();
	});

	_viewmenu->addAction("Zoom out", this, [this]()
	{
		slotzoomOut();
	});

	_viewmenu->addSeparator();
	_viewmenu->addMenu(_options );
	_viewmenu->addSeparator();
	// these three need making checkable
	previewViewID = _viewmenu->addAction( "Print Preview", this,
									   [this]()
	{
		slotPreviewView();
	});
	previewViewID->setChecked(true);

	ganttViewID = _viewmenu->addAction( "Gantt View", this,
										[this]()
	{
		slotGanttView();
	});

	taskViewID = _viewmenu->addAction( "Task Editor", this,
									   [this]()
	{
		slotTaskView();
	});

	calViewID = _viewmenu->addAction( "Calendar View", this,
									  [this]()
	{
		slotCalendarView();
	});
	calViewID->setCheckable(true);

	_menubar->addSeparator();

	_helpmenu = _menubar->addMenu("&Help");
	_helpmenu->addAction( "&About QtGantt", this,
						  [this]()
	{
		slotAbout();
	});

	//
	// Create the toolbar
	//
	/*QToolBar * toolbar = new QToolBar( this);

	QPixmap newfilePixmap = QPixmap( newfile_xpm );
	toolbar->addAction(newfilePixmap, tr("Create a new project"),
					   [this]()
	{
		slotNewFile();
	})

	QPixmap openfilePixmap = QPixmap( openfile_xpm );
	QToolButton * opentoolbutton = new QToolButton( openfilePixmap,
		"Open an existing project", 0, this, SLOT(slotOpenFile()), 
		toolbar, "Open Project");

	QPixmap savefilePixmap = QPixmap( savefile_xpm );
	QToolButton * savetoolbutton = new QToolButton( savefilePixmap,
		"Save the current project", 0, this, SLOT(slotSaveFile()), 
		toolbar, "Save Project");

	QPixmap printfilePixmap = QPixmap( printfile_xpm );
	QToolButton * printtoolbutton = new QToolButton( printfilePixmap,
		"Print the current project", 0, this, SLOT(slotPrintFile()), 
		toolbar, "Print Project");

	toolbar->addSeparator();

	QPixmap linktasksPixmap = QPixmap( linktasks_xpm );
	QToolButton * linktasksbutton = new QToolButton( linktasksPixmap,
		"Link the selected tasks", 0, this, SLOT(slotLinkTasks()), 
		toolbar, "Link Tasks");

	QPixmap unlinktasksPixmap = QPixmap( unlinktasks_xpm );
	QToolButton * unlinktasksbutton = new QToolButton( unlinktasksPixmap,
		"Unlink the selected tasks", 0, this, SLOT(slotUnlinkTasks()), 
		toolbar, "Unlink Tasks");

	toolbar->addSeparator();

	QPixmap infotaskPixmap = QPixmap( infotask_xpm );
	QToolButton * infotaskbutton = new QToolButton( infotaskPixmap,
		"Change Task Details", 0, this, SLOT(slotInfoTask()), 
		toolbar, "Task Details");

	QPixmap notetaskPixmap = QPixmap( notetask_xpm );
	QToolButton * notetaskbutton = new QToolButton( notetaskPixmap,
		"Change Task Note", 0, this, SLOT(slotNoteTask()), 
		toolbar, "Task Note");

	QPixmap aftertaskPixmap = QPixmap( aftertask_xpm );
	QToolButton * aftertaskbutton = new QToolButton( aftertaskPixmap,
		"Change Task Predecessors", 0, this, SLOT(slotAfterTask()), 
		toolbar, "Task Predecessors");

	toolbar->addSeparator();

	QPixmap gototaskPixmap = QPixmap( gototask_xpm );
	QToolButton * gototaskbutton = new QToolButton( gototaskPixmap,
		"Goto selected task", 0, this, SLOT(slotGotoTask()), 
		toolbar, "Goto selected task");

	toolbar->addSeparator();*/

	// Assign accelerators
	// Key_F5
	f5acc = new QShortcut(QKeySequence(tr("F5")), this,
						  [this]()
	{
		slotRefreshFile();
	});    // Draws gantt when F5 is pressed
	
	plusacc = new QShortcut(QKeySequence(tr("+")), this,
							[this]()
	{
		slotzoomIn();
	});    // Zooms in when + is pressed

	// Key_Minus
	minusacc = new QShortcut(QKeySequence(tr("-")), this,
							 [this]()
	{
		slotzoomOut();
	});    // Zooms out when - is pressed

	// Create scroll bar control
	//scrolltextpix = new QScrollBar( 0, 300, 1, 10, 150, 
	//		QScrollBar::Horizontal, this, "scrolltextpix" );
	//int stpW = 150;
	//scrolltextpix->setMinimumSize( stpW, ctrlH );
	//connect( scrolltextpix, SIGNAL( valueChanged(int) ), this, 
	//		SLOT(slotchangetextpix(int)) );

	//// Create zoom control
	//spindaypix = new QSpinBox( MINDAYPIX, MAXDAYPIX, 1, this, "spindaypix");
	//spindaypix->setMinimumSize( 50, ctrlH );
	//spindaypix->setValue( 10 );
	//connect( spindaypix, SIGNAL( valueChanged(int) ), this, 
	//		SLOT(slotchangedaypix(int)) );

	// Create the tabwidget. This holds all the main widgets, and the view
	// is switched between them using the menu or tab bar.
	tabwidget = new QTabWidget( this);
	tabwidget->setGeometry(2,2,10,25);
	tabwidget->setTabPosition( QTabWidget::South);
	connect(tabwidget, &QTabWidget::currentChanged, [this](int i)
	{
		slotViewTabSelected(i);
	});

	//
	// Create the statusbar
	//
	_statusbar = statusBar();
	_statusbar->showMessage("Welcome to QtGantt!", 10000);

	// Create all views
	// The Gantt object is the "old" view, useful for print preview
	/*gantt = new Gantt( project, tabwidget, "gantt" );
	tabwidget->addTab( gantt, "Print Preview" );*/

	// The GanttView is the "new" view, comprising QListView of tasks,
	// QSplitter, and GanttChart.
	ganttview = new GanttView( project, tabwidget, "ganttview" );
	tabwidget->addTab( ganttview, "Gantt View" );
	//tabwidget->showPage( ganttview );

	// The Task Editor allows editing of tasks
	taskview = new TaskView( project, tabwidget);
	tabwidget->addTab( taskview, "Task Editor" );

	// The CalendarView is another view, by time.
	calendarview = new CalendarView( project, tabwidget, "calendar" );
	tabwidget->addTab( calendarview, "Calendar" );

	setCentralWidget( tabwidget );

	//
	// Layout management
	//
	// Create a vertical layout to contain all widgets and sub-layouts.

	/*QVBoxLayout * vLayout = new QVBoxLayout( this);
	vLayout->setMenuBar( _menubar );*/

	// Create a horizontal layout to hold the toolbar, spin control and
	// zoom scroll control
	// FIXME: there's something not quite right here - this code works well
	// on QT2.1, but not on QT2.2. Could the QT experts help me please?!
	//QHBoxLayout * hTopLayout = new QHBoxLayout(this);
	//vLayout->addLayout(hTopLayout);

	////hTopLayout->addWidget( toolbar, 0 );
	//hTopLayout->addSpacing( 20 );
	//hTopLayout->addStretch( 3 );

	///*hTopLayout->addWidget( scrolltextpix, 0 );
	//hTopLayout->addSpacing( 20 );

	//hTopLayout->addWidget( spindaypix, 0 );*/

	//// Fill the rest of the horizontal box up with no stretchable space, so
	//// that the widgets get their minimum width and are pushed to the left.
	//hTopLayout->addStretch( 0 );

	// Finished with the top layout

	// The tabwidget grabs all the space in the middle. Set vertical
	// stretch to 10 to let the tabwidget stretch vertically. It will
	// stretch horizontally because there are no widgets beside it to take
	// up the horizontal stretch.
	//vLayout->addWidget( tabwidget);

	//vLayout->addWidget( _statusbar );

	//// Activate that layout!
	//vLayout->activate();
	}


void MainWidget::slottoggleweekdetail()
{
	isWeekdetail = !isWeekdetail;
	weekdetailID->setChecked(isWeekdetail);
	//gantt->setweekdetail(isWeekdetail);
	ganttview->setweekdetail(isWeekdetail);
}

void MainWidget::slotzoomIn()
{
	int s = spindaypix->value();
	if (s < MAXDAYPIX ) {
		spindaypix->setValue( ++s );
		//gantt->changedaypix(s);
		ganttview->changedaypix(s);
	}
}

void MainWidget::slotzoomOut()
{
	int s = spindaypix->value();
	if (s > MINDAYPIX ) {
		spindaypix->setValue( --s );
		//gantt->changedaypix(s);
		ganttview->changedaypix(s);
	}
}

void MainWidget::resizeEvent( QResizeEvent * )
	{
	}


void MainWidget::slotNewFile()
{
	int t;
	if ( project->isChanged() )
	{
		auto res = QMessageBox::information(this, "QtGantt",
			"This file contains unsaved work\n"
			"Do you want to save it before creating a new file?\n",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		switch (res)
		{
		case QMessageBox::Save: // Save
			slotSaveFile();
			break;
		case QMessageBox::Discard: // Don't Save
			break;
		case QMessageBox::Cancel: // Cancel
			return;
		}
	}

	// Reset and repaint
	project->reset();
	//gantt->repaint(); // may have to be refresh
	taskview->repaint();
}

void MainWidget::slotOpenFile()
{
	if ( project->isChanged() )
	{
		switch (QMessageBox::information( this, "QtGantt",
				"This file contains unsaved work\n"
				"Do you want to save it before opening another file?\n",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel))
		{
		case QMessageBox::Save: // Save
			slotSaveFile();
			break;
		case QMessageBox::Discard: // Don't Save
			break;
		case QMessageBox::Cancel: // Cancel
			return;
		}
	}

	// Open file dialog; Open file (which resets before reading)
	QString filename( QFileDialog::getOpenFileName( 0, "*.gantt" ) );
	if ( !filename.isNull() )             // if cancel was not pressed
	{
		project->openFilename( filename );
		//gantt->draw(); // may have to be refresh
		taskview->populate(true);
		ganttview->populate(true);
	}
}

void MainWidget::slotSaveFile()
{
	qDebug("slotSaveFile");
	if ( ! project->isChanged() )
	{
		QMessageBox::information( this, "QtGantt",
			"The project has not changed: no save necessary.\n");
		return;
	}

	if ( ! project->HasFilename() )
	{
		QString filename( QFileDialog::getSaveFileName( 0, "*.gantt" ) );
		if ( !filename.isNull() )	// if cancel was not pressed
		{
			if (project->SetFilename( filename ))
			{
				qDebug("SetFilename returned false");
				return;
			}
		}
	}
	else
	{
		qDebug("Calling save...");
		if (!project->save())
		{
			QMessageBox::information( this, "QtGantt",
			"The save failed.\n");
		}
		else
		{
			_statusbar->showMessage("Saved OK", 10000);
		}

		qDebug("Finished calling save...");
	}
}

void MainWidget::slotRefreshFile()
{
	if ( project->isChanged() )
	{
		switch (QMessageBox::information( this, "QtGantt",
				"This file has changed.\n"
				"Refreshing will abandon your changes.\n"
				"Do you still want to refresh?\n",
			QMessageBox::Yes | QMessageBox::No))
		{
		case QMessageBox::Yes: // Yes, reload
			break;
		case QMessageBox::No: // Don't reload
			return;
		}
	}
	project->refresh();
	//gantt->repaint(); // may have to be refresh
	taskview->redraw();
	}

void MainWidget::slotPropertiesFile()
{
	ProjectInfoDialogImpl pid( this, project );
	pid.exec();
}

void MainWidget::slotPrintFile()
{
	/*QPrinter * printer = new QPrinter();
	gantt->print( printer );*/
}


void MainWidget::slotPreviewView()
{
	// We can be invoked by the user changing the current tab, or by the
	// menu. If the user changes the tab, then the view is automatically
	// changed - we only need to change the view if we're currently showing
	// a different one.
	/*if (tabwidget->currentWidget() != gantt )
		tabwidget->setCurrentWidget( gantt );*/
	previewViewID->setChecked(true);
	ganttViewID->setChecked(false);
	taskViewID->setChecked(false);
	calViewID->setChecked(false);
	const char * msg = "If there are more tasks than can fit, they don't display correctly.";
	qWarning(msg);
	_statusbar->showMessage(msg, 10000);
}

void MainWidget::slotGanttView()
{
	// We can be invoked by the user changing the current tab, or by the
	// menu. If the user changes the tab, then the view is automatically
	// changed - we only need to change the view if we're currently showing
	// a different one.
	if (tabwidget->currentWidget() != ganttview )
		tabwidget->setCurrentWidget( ganttview );
	previewViewID->setChecked(false);
	ganttViewID->setChecked(true);
	taskViewID->setChecked(false);
	calViewID->setChecked(false);
	const char * msg = "There are slight dependency arrow misdrawings in this view";
	qWarning(msg);
	_statusbar->showMessage(msg, 10000);
}

void MainWidget::slotTaskView()
{
	// We can be invoked by the user changing the current tab, or by the
	// menu. If the user changes the tab, then the view is automatically
	// changed - we only need to change the view if we're currently showing
	// a different one.
	if (tabwidget->currentWidget() != taskview )
		tabwidget->setCurrentWidget( taskview );
	previewViewID->setChecked(false);
	ganttViewID->setChecked(false);
	taskViewID->setChecked(true);
	calViewID->setChecked(false);
	const char * msg = "Don't forget to Update Tasks when editing";
	qWarning(msg);
	_statusbar->showMessage(msg, 10000);
}

void MainWidget::slotCalendarView()
{
	// We can be invoked by the user changing the current tab, or by the
	// menu. If the user changes the tab, then the view is automatically
	// changed - we only need to change the view if we're currently showing
	// a different one.
	if (tabwidget->currentWidget() != calendarview )
		tabwidget->setCurrentWidget( calendarview );
	previewViewID->setChecked(false);
	ganttViewID->setChecked(false);
	taskViewID->setChecked(false);
	calViewID->setChecked(true);
	const char * msg = "Calendar view is unfinished";
	qWarning(msg);
	_statusbar->showMessage(msg, 10000);
}

// Invoked when the user selects a tab - we emit a signal to the appropriate
// handler above, to update the status message (and do any other tasks which may
// be needed when the tab changes)
void MainWidget::slotViewTabSelected(int /*i*/)
{
	QWidget * w = tabwidget->currentWidget();
	//if (w == gantt)
	//	slotPreviewView(); // FIX: should be emit
	//	else 
	if (w == ganttview)
		slotGanttView(); // FIX: should be emit
	else if (w == taskview)
		slotTaskView(); // FIX: should be emit
	else if (w == calendarview)
		slotCalendarView(); // FIX: should be emit
	else 
		qWarning("Odd value in slotViewTabSelected");
}

void MainWidget::slotLinkTasks()
{
	qDebug("Creating the tid");
	//TaskInfoDialogImpl tid( this, "link tasks dialog" );
	//tid.exec();
}

void MainWidget::slotUnlinkTasks()
{
}

void MainWidget::slotInfoTask()
{
}

void MainWidget::slotNoteTask()
{
}

void MainWidget::slotAfterTask()
{
}

void MainWidget::slotGotoTask()
{
}

void MainWidget::slotchangetextpix(int tpix)
{
	//gantt->changetextpix(tpix);
}

void MainWidget::slotchangedaypix(int dpix)
{
	//gantt->changedaypix(dpix);
	ganttview->changedaypix(dpix);
}



void MainWidget::slotQuit()
{
	if ( project->isChanged() )
	{
		switch (QMessageBox::information( this, "QtGantt",
				"This file contains unsaved work\n"
				"Do you want to save it before quitting?\n",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel))
		{
		case QMessageBox::Save: // Save
			slotSaveFile();
			break;
		//case 1: // Don't Save
		//	break;
		case QMessageBox::Cancel: // Cancel
			return;
		}
	}
	// emit to qApp? perhaps change this when I understand signals/slots
	// better
	qApp->quit();
}

void MainWidget::slotAbout()
{
	QMessageBox::information( this, "About QtGantt",
				"QtGantt - a project management system\n"
				"Copyright 2000 by Gunnstein Lye, Matt Gumbley\n"
				"and the gantt-devel@geekstuff.co.uk\n"
				"mailing list\n"
				);
}

