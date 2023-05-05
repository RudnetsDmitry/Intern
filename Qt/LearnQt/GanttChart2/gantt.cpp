//===========================================================================
//
//  Filename  : gantt.cpp
//  Purpose   : Gantt View for QtGantt - the original view of the project,
//              containing a hand-drawn list of tasks with chart. This will be
//              removed in later versions when the GanttView object is working
//              fully.
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

#include <qpainter.h>
//#include <qprinter.h>
//#include <qprintdialog.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qevent.h>

#include "task.h"
#include "project.h"
#include "gantt.h"

#include <QtPrintSupport/QPrinter>

// define this for Gantt paint debug messages to be output
#define G_P_DEBUG

Gantt::Gantt(Project * proj, QWidget * parent, const char * name)
	: QWidget(parent/*, name */)
{
	project = proj;
	//setBackgroundColor( "white" );
	textpixx = 150;
	daypix = 10;
	isprinting = 0;
	weekdetail = 0;
	TEXTPIXY = 50;
	Gantt::paintcount = 1;

	// The control widgets from MainWidget should move here...
}




void Gantt::print(QPrinter * printer)  // draw gantt to printer
{
	this->printer = printer;
	if (project->numtasks > 0)    // if file reading went OK
	{
		if (true /*printer->setup() */)   // if pushed OK button in printer setup
		{
			isprinting = 1;
			QPainter painter;
			painter.begin(printer);
			paint(&painter);      // calls the paint function
			painter.end();
			isprinting = 0;
		}
	}
	this->printer = 0;
}

// draw gantt to file without setup dialog
void Gantt::printToFile(QPrinter * printer, char * filename)
{
	this->printer = printer;
	if (project->numtasks > 0)    // if file reading went OK
	{
		isprinting = 1;
		QPainter painter;
		printer->setOutputFileName(filename);
		painter.begin(printer);
		paint(&painter);      // calls the paint function
		painter.end();
		isprinting = 0;
	}
	this->printer = 0;
}


void Gantt::draw()  // draw gantt to screen
{
	if (project->numtasks > 0)   // if file reading went OK
	{
		QPainter painter;
		painter.begin(this);
		paint(&painter);
		painter.end();
	}
}


void Gantt::changetextpix(int tpix)
{
	textpixx = tpix;
	repaint();
}


void Gantt::changedaypix(int dpix)
{
	daypix = dpix;
	repaint();
}

void Gantt::setweekdetail(int newweekdetail)
{
	weekdetail = newweekdetail;
	repaint();
}


void Gantt::paint(QPainter * painter)  // draw gantt, used by print() and draw()
{
	int page = 1, pagewidth, pageheight;
	int textHeight, ascent, twoDigitWidth, leading;
	int canFitDayNums = 0;
	if (isprinting)
	{
#ifdef G_P_DEBUG
		qWarning("*** PRINTING ***");
#endif
		// CHANGE: this ain't gonna work - we need to know exactly how
		// big our pages are, so we can calculate pagetasks (before main
		// loop). The Qt book has something about printing.
		pagewidth = 1200;
		pageheight = 800;
	}
	else
	{
#ifdef G_P_DEBUG
		qWarning("*** DISPLAYING ***");
#endif
		pagewidth = width();
		pageheight = height();
	}

	// Draws the header text
	painter->setPen("black");
	painter->setFont(QFont("times", 20, QFont::Bold));
	painter->drawText(10, 0, pagewidth - 20, TITLEPIXY, Qt::AlignCenter,
				*project->name);
	QFontMetrics qfm = painter->fontMetrics();
	ascent = qfm.ascent();
	textHeight = ascent + qfm.descent();

	// Draws lines for weeks, months and current date
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Normal));
	qfm = painter->fontMetrics();
	ascent = qfm.ascent();
	textHeight = ascent + qfm.descent();
	twoDigitWidth = qfm.boundingRect("33").width();
	leading = qfm.leading();

	// If we're doing week detail, the calendar header is taller, to take
	// into account the start-of-week day numbers (but only if they'll fit)
	if (weekdetail && daypix > (twoDigitWidth + 8))
	{
		TEXTPIXY = TITLEPIXY + 10 + (textHeight * 2);
		canFitDayNums = 1;
	}
	else
	{
		TEXTPIXY = TITLEPIXY + 10 + textHeight;
		canFitDayNums = 0;
	}
	int i = 0, pagetasks = 20, currentpage;
	pagetasks = (pageheight - TEXTPIXY) / SPACING;
#ifdef G_P_DEBUG
	qWarning("\n\n\npaint number %d", Gantt::paintcount++);
	qWarning("pageheight is %d, pagetasks is %d\n", pageheight, pagetasks);
#endif
	while (i < project->numtasks)
	{
		currentpage = page;
		// Top of loop - draw the background info, headings, lines,
		// shaded bits for weekends etc, for this "page"
#ifdef G_P_DEBUG
		qWarning("Drawing headings...");
#endif
		// Draw top border around months and optional start-of-week day
		// numbers
		painter->setPen("darkgrey");
		painter->drawLine(0, TITLEPIXY, pagewidth, TITLEPIXY);
		painter->drawLine(0, TEXTPIXY, pagewidth, TEXTPIXY);

		// Draws lines for weeks, months and current date
		painter->setFont(QFont("helvetica", FONTSIZE, QFont::Normal));
		//QFontMetrics qfm = painter->fontMetrics();
		//ascent = qfm.ascent();
		//textHeight = ascent + qfm.descent();
		QDate tempdate = project->startdate->addDays(0);
		for (int day = textpixx; day < pagewidth; day = day + daypix)
		{
			// fill in weekends with light grey
			if (weekdetail &&
				(tempdate.dayOfWeek() == 6 ||
				 tempdate.dayOfWeek() == 7))
			{
				painter->setPen("lightgrey");
				painter->setBrush(QBrush(QColor("lightgrey")));
				painter->drawRect(day, TEXTPIXY + 1, daypix, pageheight);
			}
			// indicate Mondays with a grey line
			if (weekdetail || tempdate.dayOfWeek() == 1)  // if a monday
			{
				QPen dottedpen(weekdetail ? QBrush(QColor("black")) : QBrush(QColor("lightgrey")), 0, Qt::DashLine);
				painter->setPen(dottedpen);
				painter->drawLine(day, TEXTPIXY, day, pageheight);  // Draws a dotted line for each week
				// display the day number for the start of each
				// week
				if (canFitDayNums && tempdate.dayOfWeek() == 1) // weekdetail is implied by canFitDayNums
				{
					painter->setPen("black");
					QString qstr;
					int dayNoY = TITLEPIXY + 5 + textHeight + ascent + leading;
					painter->drawText(day + 5, dayNoY, qstr.setNum(tempdate.day()));
				}
			}
			if (tempdate.day() == 1)    // if first day of month
			{
				painter->setPen("darkgrey");
				painter->drawLine(day, TITLEPIXY, day, pageheight);  // Draws a solid line for each month
				painter->setPen("black");
				int monthY = TITLEPIXY + 5 + ascent;
				switch (tempdate.month())
				{
					// these drawTexts are baseline-relative.
				case 1: painter->drawText(day + 5, monthY, "January"); break;
				case 2: painter->drawText(day + 5, monthY, "February"); break;
				case 3: painter->drawText(day + 5, monthY, "March"); break;
				case 4: painter->drawText(day + 5, monthY, "April"); break;
				case 5: painter->drawText(day + 5, monthY, "May"); break;
				case 6: painter->drawText(day + 5, monthY, "June"); break;
				case 7: painter->drawText(day + 5, monthY, "July"); break;
				case 8: painter->drawText(day + 5, monthY, "August"); break;
				case 9: painter->drawText(day + 5, monthY, "September"); break;
				case 10: painter->drawText(day + 5, monthY, "October"); break;
				case 11: painter->drawText(day + 5, monthY, "November"); break;
				case 12: painter->drawText(day + 5, monthY, "December"); break;
				}
			}
			if (tempdate == tempdate.currentDate())    // if current date
			{
				painter->setPen("red");
				// Draw a solid red line for the current date
				painter->drawLine(day, TEXTPIXY + 1, day, pageheight);
				if (weekdetail)
				{
					painter->setBrush(QBrush(QColor("pink")));
					painter->drawRect(day, TEXTPIXY + 1, daypix, pageheight);
				}
			}
			tempdate = tempdate.addDays(1);
		}

		// Body of loop
		// Draws the tasks, mains and milestones
		for (i; i < project->numtasks && page == currentpage; i++)  // for each task
		{
#ifdef G_P_DEBUG
			qWarning("top of loop i is %d, page is %d, currentpage is %d, i%%pagetasks is %d",
					i, page, currentpage, i % pagetasks);
#endif

			// Switch to a new page, if we've filled this one (only
			// when printing)
			if (i != 0 && (i % pagetasks) == 0)
			{
#ifdef G_P_DEBUG
				qWarning("new page\n");
#endif
				if (isprinting)
					printer->newPage();
				page++;
			}

			if (page == currentpage && project->tasks[i])   // if this tasknumber is read from file
			{
#ifdef G_P_DEBUG
				qWarning("drawing task\n");
#endif
				int j = 0;
				if (project->tasks[i]->after != 0)	// if 'after' is specified (which means the task depends on other tasks)
				{			// search for the last other task (only have one dependency for now)
					while (project->tasks[j]->num != project->tasks[i]->after && j < i)
					{
						j++;
					}
				}

#ifdef G_P_DEBUG
				qWarning("i is %d, j is %d\n", i, j);
#endif
				int jbegin, jend;
				// If the Y position of the current task (i) is after j's
				// use 0/1200, else use j's "proper position"
				// WHAT DOES THIS DO?
				if ((int)((float)(i / pagetasks) - (float)(j / pagetasks)) > 0)
				{
					qDebug("doing that bit I don't understand");
					jbegin = 0;
					jend = 1200;
				}
				else
				{
					jbegin = j % pagetasks;
					jend = j % pagetasks;
				}
#ifdef G_P_DEBUG
				qWarning("jbegin is %d, jend is %d\n", jbegin, jend);
#endif
				// Shift the task detail up a little
				TEXTPIXY -= 4;

				// Paint elements (task/main/mile) and end arrows for the task
				// in question (i)
				if (project->tasks[i]->type == TASK)   // draws a normal task
				{
					painttask(painter, i, i % pagetasks);
					if (project->tasks[i]->after != 0)                     // if 'after' is specified (which means the task depends on other tasks)
					{
						painttaskendarrow(painter, i, i % pagetasks, jend);
					}   // draws first part of the dependency arrow
				}
				else if (project->tasks[i]->type == MAIN)   // draws a main task
				{
					paintmain(painter, i, i % pagetasks);
					if (project->tasks[i]->after != 0)                     // if 'after' is specified (which means the task depends on other tasks)
					{
						paintmainendarrow(painter, i, i % pagetasks, jend);
					}   // draws first part of the dependency arrow
				}
				else if (project->tasks[i]->type == MILE)  // draws a milestone
				{
					paintmile(painter, i, i % pagetasks);
					if (project->tasks[i]->after != 0)                     // if 'after' is specified (which means the task depends on other tasks)
					{
						paintmileendarrow(painter, i, i % pagetasks, jend);
					}   // draws first part of the dependency arrow
				}
				// Paint any dependency arrows for the task depended upon (j)
				if (project->tasks[j]->type == TASK &&
					project->tasks[i]->after != 0)   // draws a normal task
				{
					painttaskbeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
				}
				else if (project->tasks[j]->type == MAIN &&
					 project->tasks[i]->after != 0)   // draws a main task
				{
					paintmainbeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
				}
				else if (project->tasks[j]->type == MILE &&
					 project->tasks[i]->after != 0)  // draws a milestone
				{
					paintmilebeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
				}
			}
			// Shift the task detail back down
			TEXTPIXY += 4;
#ifdef G_P_DEBUG
			qWarning("drawing grid\n");
#endif

			// draw the grid line
			painter->setPen("lightgrey");
			painter->drawLine(0, TEXTPIXY + ((i + 1) % pagetasks) * SPACING, pagewidth, TEXTPIXY + ((i + 1) % pagetasks) * SPACING);


		}
	}
}


void Gantt::paintEvent(QPaintEvent * e)  // repaints automagically when needed
{
	draw();
}


void Gantt::painttask(QPainter * painter, int taskno, int y)
{
	int taskX, taskY, taskW, taskWE, taskH;
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("lightblue")));

	// Write description
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Normal));
	painter->drawText(10, TEXTPIXY + 13 + y * SPACING,
					   textpixx - 10, 20, Qt::AlignLeft,
					   *project->tasks[taskno]->description);

	// Draw rectangle; take percentage completed into account
	taskX = textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	taskY = TEXTPIXY + 10 + y * SPACING;
	// taskW is the full width of the task from start to end
	taskW = daypix * project->tasks[taskno]->startdate->daysTo(project->tasks[taskno]->enddate->addDays(0));
	// taskWE is the full width of the task from start to expected end. End
	// can overrun the expected end.
	taskWE = daypix * project->tasks[taskno]->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
	// taskH is the height of the bars, and is related to the spacing
	taskH = SPACING - 10;
	/*
	 * The new code doesn't work quite as well as I'd like - quicker, but
	 * sometimes the blue and red parts don't quite meet.
	painter->setBrush( "red" );
	painter->drawRect( taskX, taskY, taskW, taskH);
	painter->setBrush( "lightblue" );
	painter->drawRect( taskX, taskY, taskWE, taskH);
	*/
	// Draw the expected task duration in blue, and any overtime in red
	painter->setBrush(QBrush(QColor("lightblue")));
	painter->drawRect(taskX, taskY, taskWE, taskH);
	painter->setBrush(QBrush(QColor("red")));
	painter->drawRect(taskX + taskWE, taskY, taskW - taskWE, taskH);
	if (project->tasks[taskno]->completed > 0)
	{
		painter->setBrush(QBrush(QColor("blue")));
		painter->setPen("blue");
		// taskWcomp is the width that has been completed
		int taskWcomp = (taskW * project->tasks[taskno]->completed) / 100;
		painter->drawRect(taskX + 1, taskY + (taskH / 3), taskWcomp - 2, (taskH / 3));
	}
}


void Gantt::paintmain(QPainter * painter, int taskno, int y)
{
	int xpos, ypos;
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("black")));
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Bold));
	painter->drawText(10, TEXTPIXY + 13 + y * SPACING,
					   textpixx - 10, 20, Qt::AlignLeft,
					   *project->tasks[taskno]->description);
	ypos = TEXTPIXY + y * SPACING;

	QVector<QPoint> a;

	if (project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->enddate->addDays(0)) != 0)
	{
		xpos = textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
		int expenddays = project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
		int enddays = project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->enddate->addDays(0));
		a = {
			{xpos + daypix * expenddays - 5, ypos + 15},
			{xpos + daypix * enddays + 5, ypos + 15},
			{xpos + daypix * enddays + 5, ypos + 20},
			{xpos + daypix * enddays, ypos + 25},
			{xpos + daypix * enddays - 5, ypos + 20},
			{xpos + daypix * expenddays + 5, ypos + 20},
			{xpos + daypix * expenddays, ypos + 25},
			{xpos + daypix * expenddays - 5, ypos + 20}
		};
		painter->setBrush(QBrush(QColor("red")));
		painter->drawPolygon(a);
	}

	painter->setBrush(QBrush(QColor("black")));
	int startdays = project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int expenddays = project->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
	a =
	{{textpixx + daypix * startdays - 5, ypos + 15},
		{textpixx + daypix * expenddays + 5, ypos + 15},
		{textpixx + daypix * expenddays + 5, ypos + 20},
		{textpixx + daypix * expenddays, ypos + 25},
		{textpixx + daypix * expenddays - 5, ypos + 20},
		{textpixx + daypix * startdays + 5, ypos + 20},
		{textpixx + daypix * startdays, ypos + 25},
		{textpixx + daypix * startdays - 5, ypos + 20}
	};
	painter->drawPolygon(a);
}



void Gantt::paintmile(QPainter * painter, int taskno, int y)
{
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("black")));
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Normal, 1));
	painter->drawText(10, TEXTPIXY + 13 + y * SPACING,
					   textpixx - 10, 20, Qt::AlignLeft,
					   *project->tasks[taskno]->description);
	auto daysTo = static_cast<int>(project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)));
	QVector<QPoint> a =
	{
		{textpixx + daypix * daysTo, TEXTPIXY + 15 + y * SPACING},
		{textpixx + daypix * daysTo + 5, TEXTPIXY + 20 + y * SPACING},
		{textpixx + daypix * daysTo, TEXTPIXY + 25 + y * SPACING},
		{textpixx + daypix * daysTo - 5, TEXTPIXY + 20 + y * SPACING}
	};
	painter->drawPolygon(a);
	QString qstr;
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Bold));
	painter->drawText(textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)) + 10,
					   TEXTPIXY + 25 + y * SPACING,
					   qstr.setNum(project->tasks[taskno]->startdate->day()
					   ) + "." + qstr.setNum(project->tasks[taskno]->startdate->month()));
}


void Gantt::painttaskbeginarrow(QPainter * painter, int taskno, int y, int j)
{
	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < 7)
	{
		// dependency feeds back under the task
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 17 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 17 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 17 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 27 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 27 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
	else
	{
		// dependency is "away from" the task
		// across..
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 17 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 17 + j * SPACING);
		// and down.
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 17 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
}


void Gantt::painttaskendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("black")));
	auto daysTo = project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));

	painter->drawLine(textpixx + daypix * daysTo, TEXTPIXY + 27 + j * SPACING,
					   textpixx + daypix * daysTo, TEXTPIXY + 3 + y * SPACING);

	auto iDaysTo = static_cast<int>(daysTo);
	QVector<QPoint> a =
	{
		{textpixx + daypix * iDaysTo - 5, TEXTPIXY + 4 + y * SPACING},
		{textpixx + daypix * iDaysTo + 5, TEXTPIXY + 4 + y * SPACING},
		{textpixx + daypix * iDaysTo, TEXTPIXY + 9 + y * SPACING}
	};
	painter->drawPolygon(a);
}


void Gantt::paintmainbeginarrow(QPainter * painter, int taskno, int y, int j)
{
	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < 7)
	{
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 26 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
	else
	{
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 18 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 18 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 18 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
}


void Gantt::paintmainendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("black")));
	auto daysTo = project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));

	painter->drawLine(textpixx + daypix * daysTo, TEXTPIXY + 27 + j * SPACING,
					   textpixx + daypix * daysTo, TEXTPIXY + 8 + y * SPACING);
	auto iDaysTo = static_cast<int>(daysTo);
	QVector<QPoint> a =
	{
		{textpixx + daypix * iDaysTo - 5, TEXTPIXY + 9 + y * SPACING},
		{textpixx + daypix * iDaysTo + 5, TEXTPIXY + 9 + y * SPACING},
		{textpixx + daypix * iDaysTo, TEXTPIXY + 14 + y * SPACING}
	};
	painter->drawPolygon(a);
}


void Gantt::paintmilebeginarrow(QPainter * painter, int taskno, int y, int j)
{
	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < 7)
	{
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 20 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 20 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 20 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 27 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)) + 7,
						   TEXTPIXY + 27 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
	else
	{
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0)),
						   TEXTPIXY + 20 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 20 + j * SPACING);
		painter->drawLine(textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 20 + j * SPACING,
						   textpixx + daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0)),
						   TEXTPIXY + 27 + j * SPACING);
	}
}


void Gantt::paintmileendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(QBrush(QColor("black")));
	auto daysTo = project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));

	painter->drawLine(textpixx + daypix * daysTo, TEXTPIXY + 27 + j * SPACING,
					   textpixx + daypix * daysTo, TEXTPIXY + 7 + y * SPACING);

	auto iDaysTo = static_cast<int>(daysTo);
	QVector<QPoint> a =
	{
		{textpixx + daypix * iDaysTo - 5, TEXTPIXY + 8 + y * SPACING},
		{textpixx + daypix * iDaysTo + 5, TEXTPIXY + 8 + y * SPACING},
		{textpixx + daypix * iDaysTo,TEXTPIXY + 13 + y * SPACING}
	};
	painter->drawPolygon(a);
}
