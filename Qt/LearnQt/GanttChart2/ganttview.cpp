//===========================================================================
//
//  Filename  : ganttview.cpp
//  Purpose   : Composite Gantt View for QtGantt. Comprises a QListView, a
//              QSplitter and a GanttChart object. This is the "new" view of the
//              Gantt chart/table.
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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qsplitter.h>
#include <qstring.h>
#include <qlayout.h>

#include "task.h"
#include "project.h"
#include "ganttview.h"
#include "ganttchart.h"
//#include "taskinfodialogimpl.h"

GanttView::GanttView ( Project *proj, QWidget *parent, const char *name )
    : QWidget( parent/*, name */)
{
	project = proj;

	// I have given up with splitters for the time being - they're just too
	// awkward. or is it that there's something I'm not understanding (more
	// likely!)
	//
	//splitter = new QSplitter( this );

	// Task table creation
	QFontMetrics fm( fontMetrics() );
	//m_tableView = new QListView( splitter, "gantt table" );
	m_tableView = new QTableWidget( this);
	m_tableView->setColumnCount(3);

	m_tableView->setObjectName("gantt table");
	m_tableView->setSortingEnabled(false);

	auto * horzHeader = m_tableView->horizontalHeader();
	m_tableView->setHorizontalHeaderItem(0, new QTableWidgetItem("Task ID"));

	auto * item1 = new QTableWidgetItem("Description");
	item1->setTextAlignment(Qt::AlignLeft);
	m_tableView->setHorizontalHeaderItem(1, item1);
	//m_tableView->setColumnAlignment(1, AlignLeft);

	auto * item2 = new QTableWidgetItem("Duration");
	item2->setTextAlignment(Qt::AlignRight);
	m_tableView->setHorizontalHeaderItem(2, item2);

	m_tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	// The middle column is truncated to fit between the left and right
	// columns
	int two_outsides = m_tableView->columnWidth(0) + m_tableView->columnWidth(2);
	// KLUDGE! the width of the main widget is hard-coded to 790 in
	// main.cpp, and the m_tableView is half of that. I haven't been able to
	// determine the width of the m_tableView at all - m_tableView->width() gives
	// 100, which just ain't right.
	m_tableView->setColumnWidth(1, (790/2) - two_outsides);

	m_tableView->setFocusPolicy(Qt::StrongFocus);
	//m_tableView->setAllColumnsShowFocus( TRUE );
	//m_tableView->setSorting( -1 );

	// Disallow sorting of the table by clicking on headers
	/*QHeader *qh = m_tableView->header();
	QObject::disconnect(qh, SIGNAL(sectionClicked( int )),
		m_tableView, SLOT(changeSortColumn( int )) );*/

	// Selection/deselection of tasks cause toolbar enable/disables
	QObject::connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged, [this](auto const &, auto const &)
	{
		slotSelectionChanged();
	});

	// Double click causes the task information dialog to pop up for that
	// task
	QObject::connect(m_tableView, &QTableWidget::cellDoubleClicked, [this](int row, int column)
	{
		slotTaskInfo(row, column);
	});

	//scrollview = new QScrollView( splitter );
	scrollview = new QScrollArea( this );
	// The scrollview vertical scrolling will be managed by listening to
	// movements in the m_tableView's vertical scroll bar. Hopefully!
	scrollview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// Qt Experts: how do you make the scrollview take up all the space on
	// the RHS of the splitter?
	ganttchart = new GanttChart( project, this, "gantt chart" );

	// a little kludge to determine the height of list items...
	clearTable();
	m_tableView->insertRow(0);
	int taskheight = m_tableView->rowHeight(0);
	clearTable();
	ganttchart->setTaskHeight( taskheight );

	// The following should work, but seems to return a height that is
	// greater than the header... so kludge it for now.
	//ganttchart->setHeaderHeight( m_tableView->header()->height() );
	ganttchart->setHeaderHeight( m_tableView->horizontalHeader()->height());

	//ganttchart->setBackgroundColor( "white" );
	scrollview->setWidget(ganttchart);

	QHBoxLayout *hLayout = new QHBoxLayout( this );
	hLayout->addWidget(m_tableView);
	hLayout->addWidget(scrollview);
	hLayout->activate();
}


GanttView::~GanttView ()
{
	if (m_tableView)
		delete m_tableView;
	if (ganttchart)
		delete ganttchart;
}

void GanttView::clearTable()
{
	m_tableView->clearContents();
	int rowCount = m_tableView->rowCount();
	while (rowCount > 0)
		m_tableView->removeRow(--rowCount);
}

void GanttView::populate(bool force /*= false*/)
{
	if (force)
	{
		clearTable();
		ASSERT(m_tableView->rowCount() == 0);
	}

	if (m_tableView->rowCount() > 0)
		return;

	//connect(m_tableView, SIGNAL(doubleClicked(QListViewItem *)), this,
	//		SLOT(handleClick(QListViewItem *)));
	// populating the list
	if (!this->project)
		return;

	// Qstring for set text in QListViewItem

	for (int cpt = 0; cpt < project->numtasks; ++cpt)
	{
		m_tableView->insertRow(cpt);

		m_tableView->setItem(cpt, 0, new QTableWidgetItem(QString::number(project->tasks[cpt]->num)));
		m_tableView->setItem(cpt, 1, new QTableWidgetItem(*project->tasks[cpt]->description));
		m_tableView->setItem(cpt, 2, new QTableWidgetItem(QString::number(project->tasks[cpt]->durationlength)));
	}

	if (m_tableView->rowCount() == 0)
		return;

	auto * horzHeader = m_tableView->horizontalHeader();
	horzHeader->setSectionResizeMode(0, QHeaderView::Stretch);
	horzHeader->setSectionResizeMode(1, QHeaderView::Interactive);
	horzHeader->setSectionResizeMode(2, QHeaderView::Stretch);
}

void GanttView::redraw()
{
	this->populate();

	/*m_tableView->setMinimumSize( 400,this->height());
	ganttchart->setMinimumSize( (this->width()-400),this->height() );*/
	//	m_tableView->repaint();

}


//void GanttView::paint( QPainter *painter )
//{
//	//m_tableView->setMinimumSize( 400,this->height());
//	//ganttchart->setMinimumSize( (this->width()-400),this->height() );
//	////	m_tableView->repaint();
//	//this->populate();
//	QWidget::paint(painter);
//}

void GanttView::resizeEvent( QResizeEvent * )
{
	this->redraw();
	ganttchart->setMinimumSize((this->width() - 400), this->height());
}

void GanttView::changedaypix( int dpix )
{
	ganttchart->changedaypix( dpix );
}

void GanttView::setweekdetail( int newweekdetail )
{
	ganttchart->setweekdetail( newweekdetail );
}


void GanttView::slotSelectionChanged( )
{
	qDebug("GanttView::slotSelectionChanged");
}

void GanttView::slotTaskInfo(int row, int column)
{
	ASSERT(false);
	/*qDebug("GanttView::slotTaskInfo, Task # %d", row);
	TaskInfoDialogImpl tid( this, "task info" );
	tid.SetView( TaskInfoDialogImpl::GENERAL );
	tid.SetTask(project->tasks[row]);
	tid.exec();*/
}

