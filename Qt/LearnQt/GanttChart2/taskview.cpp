//===========================================================================
//
//  Filename  : taskview.cpp
//  Purpose   : Task editor Widget for QtGantt
//  Copyright : Copyright (C) 2000 Olivier Berrod <obe@alcove.fr>
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

#include <stdio.h>

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>

#include <qlistview.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qsplitter.h>
#include <qstring.h>

 
#include "task.h"
#include "project.h"
#include "taskview.h"

TaskView::TaskView ( Project *proj, QWidget *parent)
    : QWidget( parent)
{

	project = proj;


	// Get the heignt of the widget.
	int fh = this->height();

	splitter = new QSplitter( this );

	// Task view creation

	QFontMetrics fm( fontMetrics() );
	m_tableWidget = new QTableWidget(splitter);
	m_tableWidget->setColumnCount(3);

	m_tableWidget->setSortingEnabled(false);

	auto * horzHeader = m_tableWidget->horizontalHeader();
	m_tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Task ID"));

	auto * item1 = new QTableWidgetItem("Description");
	item1->setTextAlignment(Qt::AlignLeft);
	m_tableWidget->setHorizontalHeaderItem(1, item1);
	//m_tableView->setColumnAlignment(1, AlignLeft);

	auto * item2 = new QTableWidgetItem("Duration");
	item2->setTextAlignment(Qt::AlignRight);
	m_tableWidget->setHorizontalHeaderItem(2, item2);

	m_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

	// The middle column is truncated to fit between the left and right
	// columns
	int two_outsides = m_tableWidget->columnWidth(0) + m_tableWidget->columnWidth(2);
	// KLUDGE! the width of the main widget is hard-coded to 790 in main.cpp, 
	// and the m_tableWidget is half of that. I haven't been able to determine
	// the width of the m_tableWidget at all - m_tableWidget->width() gives 100, which
	// just ain't right.
	m_tableWidget->setColumnWidth(1, (790/2) - two_outsides);
	m_tableWidget->setMinimumSize( 400, this->height() );

	m_tableWidget->setFocusPolicy(Qt::StrongFocus );
	/*m_tableWidget->setAllColumnsShowFocus( TRUE );
	m_tableWidget->setSorting( -1 );*/

	// Detail view creation
	detailview = new TaskField( splitter);
	//detailview->setBackgroundColor("white");
	detailview->setMinimumSize( 400, this->height() );
}

TaskView::~TaskView ()
{
	if (m_tableWidget)
		delete m_tableWidget;
	if (detailview)
		delete detailview;
}

void TaskView::populate(bool force /*= false*/)
{
	if (force)
	{
		clearTable();
		ASSERT(m_tableWidget->rowCount() == 0);
	}

	if (m_tableWidget->rowCount() > 0)
		return;

	QObject::connect(m_tableWidget, &QTableWidget::cellDoubleClicked, [this](int row, int column)
	{
		handleClick(row, column);
	});
	// populating the list
	if (!this->project)
		return;

	for (int cpt = 0; cpt < project->numtasks; ++cpt)
	{
		m_tableWidget->insertRow(cpt);

		m_tableWidget->setItem(cpt, 0, new QTableWidgetItem(QString::number(project->tasks[cpt]->num)));
		m_tableWidget->setItem(cpt, 1, new QTableWidgetItem(*project->tasks[cpt]->description));
		m_tableWidget->setItem(cpt, 2, new QTableWidgetItem(QString::number(project->tasks[cpt]->durationlength)));
	}

	if (m_tableWidget->rowCount() == 0)
		return;

	auto * horzHeader = m_tableWidget->horizontalHeader();
	horzHeader->setSectionResizeMode(0, QHeaderView::Stretch);
	horzHeader->setSectionResizeMode(1, QHeaderView::Interactive);
	horzHeader->setSectionResizeMode(2, QHeaderView::Stretch);
}

void TaskView::redraw()
{
	/*m_tableWidget->setMinimumSize( 400,this->height());
	detailview->setMinimumSize( (this->width()-400),this->height() );*/
	//	m_tableWidget->repaint();
	this->populate();
}

// Slot
void TaskView::handleClick(int row, int column)
{
    detailview->SetTask(project->tasks[row]);
    detailview->RefreshValues();
}

void TaskView::paint( QPainter *painter )
{
	//m_tableWidget->setMinimumSize( 400,this->height());
	//detailview->setMinimumSize( (this->width()-400),this->height() );
	////	m_tableWidget->repaint();
	//this->populate();
}

void TaskView::resizeEvent( QResizeEvent * )
{
	this->redraw();
	detailview->setMinimumSize((this->width() - 400), this->height());
}

void TaskView::clearTable()
{
	m_tableWidget->clearContents();
	int rowCount = m_tableWidget->rowCount();
	while (rowCount > 0)
		m_tableWidget->removeRow(--rowCount);
}



