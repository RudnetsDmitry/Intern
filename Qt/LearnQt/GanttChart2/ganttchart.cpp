//===========================================================================
//
//  Filename  : ganttchart.cpp
//  Purpose   : Gantt Chart Widget for QtGantt. Displays JUST a Gantt chart -
//              the list of tasks and splitter are part of the GanttView 
//              object. Gantt is the "old" view.
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
//#include <unistd.h>
//#include <iostream.h>
#include <qfile.h>
#include <qtextstream.h>

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdatetime.h>

#include "task.h"
#include "project.h"
#include "ganttchart.h"

//#define GC_P_DEBUG

namespace
{
	QBrush colorStr2Brush(QString const & color)
	{
		return QBrush(QColor(color));
	}
}

GanttChart::GanttChart(Project * proj, QWidget * parent, const char * name)
	: QWidget(parent/*, name*/)
{
	//setBackgroundColor( "white" );
	paintcount = 1;
	daypix = 10;
	weekdetail = 0;
	TEXTPIXY = 50;
	SPACING = 25;
	project = proj;
	resizeProject();
}

GanttChart::~GanttChart()
{}

void GanttChart::resizeProject()
{
	int pheight, pwidth;
	int numtasks;
	QDate * startdate = new QDate();
	QDate * finaldate = new QDate();
	project->GetStartDate(startdate);
	project->GetEndDate(finaldate);
	numtasks = project->GetNumTasks();
	pheight = TEXTPIXY + numtasks * SPACING;
	int projectdays = startdate->daysTo(finaldate->addDays(0));
	pwidth = daypix * projectdays;
	// need to add a bit to that for task dependency arrow lines which go past
	// the end of the tasks
	/*
	qDebug("GanttChart::resizeProject(%d,%d) project is %d days/%d tasks long", pwidth, pheight, projectdays, numtasks);
	qDebug("TEXTPIXY is %d, SPACING is %d", TEXTPIXY, SPACING);
	*/
	resize(pwidth, pheight);
}

void GanttChart::setHeaderHeight(int h)
{
	TEXTPIXY = h;
	resizeProject();
}

void GanttChart::setTaskHeight(int h)
{
	SPACING = h;
	resizeProject();
}


void GanttChart::setStartTaskIndex(int t)
{
	start_task = t;
}

void GanttChart::draw()  // draw gantt to screen
{
	if (project->numtasks > 0)   // if file reading went OK
	{
		QPainter painter;
		painter.begin(this);
		paint(&painter);
		painter.end();
	}
}


void GanttChart::paint(QPainter * painter)
{
	int pagewidth, pageheight;
	int textHeight, ascent, twoDigitWidth, leading;
	int canFitDayNums = 0;
	int monthY, dayNoY, calguard;

	pagewidth = width();
	pageheight = height();

	// Draws the header text
	painter->setPen("black");

	QFontMetrics qfm = painter->fontMetrics();
	ascent = qfm.ascent();
	textHeight = ascent + qfm.descent();

	// Draws lines for weeks, months and current date
	// (To be replaced with a TimeStrip widget)
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
		calguard = 2;
		monthY = calguard + ascent;
		dayNoY = SPACING - calguard - textHeight + ascent;
		canFitDayNums = 1;
	}
	else
	{
		// monthY and dayNoY is baseline posn of text
		calguard = 5;
		monthY = (SPACING / 2) - (textHeight / 2) + ascent;
		dayNoY = 0; // this isn't needed if we're not displaying week detail
		canFitDayNums = 0;
	}

	int pagetasks = 20;
	pagetasks = (pageheight - TEXTPIXY) / SPACING;
#ifdef GC_P_DEBUG
	qWarning("\n\n\npaint number %d\n\n\n", paintcount++);
	qWarning("pageheight is %d, pagetasks is %d\n", pageheight, pagetasks);
#endif

	// Top of loop - draw the background info, headings, lines,
	// shaded bits for weekends etc, for this "page"
	// Draw top border around months and optional start-of-week day
	// numbers
	painter->setPen("darkgrey");
	painter->drawLine(0, TEXTPIXY, pagewidth, TEXTPIXY);

	// Draws vertical lines for weeks, months and current date
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Normal));
	QDate tempdate = project->startdate->addDays(0);
	for (int day = 0; day < pagewidth; day += daypix)
	{
		// fill in weekends with light grey
		if (weekdetail &&
			(tempdate.dayOfWeek() == 6 ||
			 tempdate.dayOfWeek() == 7))
		{
			painter->setPen("lightgrey");
			painter->setBrush(colorStr2Brush("lightgrey"));
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
				painter->drawText(day + calguard, dayNoY, qstr.setNum(tempdate.day()));
			}
		}
		if (tempdate.day() == 1)    // if first day of month
		{
			painter->setPen("darkgrey");
			painter->drawLine(day, 0, day, pageheight);  // Draws a solid line for each month
			painter->setPen("black");
			switch (tempdate.month())
			{
				// these drawTexts are baseline-relative.
				// We should really use tr() on the month names
			case 1: painter->drawText(day + calguard, monthY, "January"); break;
			case 2: painter->drawText(day + calguard, monthY, "February"); break;
			case 3: painter->drawText(day + calguard, monthY, "March"); break;
			case 4: painter->drawText(day + calguard, monthY, "April"); break;
			case 5: painter->drawText(day + calguard, monthY, "May"); break;
			case 6: painter->drawText(day + calguard, monthY, "June"); break;
			case 7: painter->drawText(day + calguard, monthY, "July"); break;
			case 8: painter->drawText(day + calguard, monthY, "August"); break;
			case 9: painter->drawText(day + calguard, monthY, "September"); break;
			case 10: painter->drawText(day + calguard, monthY, "October"); break;
			case 11: painter->drawText(day + calguard, monthY, "November"); break;
			case 12: painter->drawText(day + calguard, monthY, "December"); break;
			}
		}
		if (tempdate == tempdate.currentDate())    // if current date
		{
			painter->setPen("red");
			painter->drawLine(day, TEXTPIXY + 1, day, pageheight);  // Draws a solid red line for the current date
			if (weekdetail)
			{
				painter->setBrush(colorStr2Brush("pink"));
				painter->drawRect(day, TEXTPIXY + 1, daypix, pageheight);
			}
		}
		tempdate = tempdate.addDays(1);
	}

	// Body of loop
	// Draws the tasks, mains and milestones
	int drawn = 0;
	// assume for now that our topmost task is task 0 - this'll change as
	// they scroll through the listview.
	int i = 0; // FIX: take this from the qlistview's topmost entry
	for (; i < project->numtasks && drawn < pagetasks; i++)  // for each task
	{
		//printf("top of loop i is %d, i%%pagetasks is %d\n", i, i%pagetasks);

				// draw the grid line
		painter->setPen("lightgrey");
		//painter->drawLine( 0, TEXTPIXY+((i+1)%pagetasks)*SPACING, pagewidth, TEXTPIXY+((i+1)%pagetasks)*SPACING );
		painter->drawLine(0, TEXTPIXY + (i + 1) * SPACING, pagewidth, TEXTPIXY + (i + 1) * SPACING);

		// Only draw if this tasknumber is read from file and is not collapsed (that bit comes later)
		if (project->tasks[i])
		{
			//printf("drawing task\n");
			int j = 0;
			if (project->tasks[i]->after != 0)	// if 'after' is specified (which means the task depends on other tasks)
			{			// search for the last other task (only have one dependency for now)
				while (project->tasks[j]->num != project->tasks[i]->after && j < i)
				{
					j++;
				}
			}

			//printf("i is %d, j is %d\n", i, j);
			int jbegin, jend;
			// If the Y position of the current task (i) is after j's
			// use 0/1200, else use j's "proper position"
			// WHAT DOES THIS DO?
			if ((int)((float)(i / pagetasks) - (float)(j / pagetasks)) > 0)
			{
				jbegin = 0;
				jend = 1200;
			}
			else
			{
				jbegin = j % pagetasks;
				jend = j % pagetasks;
			}
			//printf("jbegin is %d, jend is %d\n", jbegin, jend);

						// Paint elements (task/main/mile) and end arrows for the task in
						// question (i)
			if (project->tasks[i]->type == TASK)
			{
				painttask(painter, i, i % pagetasks);
				if (project->tasks[i]->after != 0)
					// if 'after' is specified, the task depends on other tasks
				{
					painttaskendarrow(painter, i, i % pagetasks, jend);
				}   // draws end of the dependency arrow
			}

			else if (project->tasks[i]->type == MAIN)
			{
				paintmain(painter, i, i % pagetasks);
				if (project->tasks[i]->after != 0)
					// if 'after' is specified, the task depends on other tasks
				{
					paintmainendarrow(painter, i, i % pagetasks, jend);
				}   // draws end of the dependency arrow
			}

			else if (project->tasks[i]->type == MILE)
			{
				paintmile(painter, i, i % pagetasks);
				if (project->tasks[i]->after != 0)
					// if 'after' is specified, the task depends on other tasks
				{
					paintmileendarrow(painter, i, i % pagetasks, jend);
				}   // draws end of the dependency arrow
			}

			// Paint any dependency arrows for the task depended upon (j)
			if (project->tasks[j]->type == TASK && project->tasks[i]->after != 0)
			{
				painttaskbeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
			}

			else if (project->tasks[j]->type == MAIN && project->tasks[i]->after != 0)
			{
				paintmainbeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
			}

			else if (project->tasks[j]->type == MILE && project->tasks[i]->after != 0)
			{
				paintmilebeginarrow(painter, i, i % pagetasks, jbegin);   // draws first part of the dependency arrow
			}

			drawn++;
		}
	}
}

void GanttChart::changedaypix(int dpix)
{
	daypix = dpix;
	resizeProject();
	repaint();
}

void GanttChart::setweekdetail(int newweekdetail)
{
	weekdetail = newweekdetail;
	repaint();
}



void GanttChart::paintEvent(QPaintEvent * e)  // repaints automagically when needed
{
	draw();
}


void GanttChart::painttask(QPainter * painter, int taskno, int y)
{
	int taskX, taskY, taskW, taskWE, taskH;
	painter->setPen("black");
	painter->setBrush(colorStr2Brush("lightblue"));

	// Draw rectangle; take percentage completed into account
	taskX = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int fifthSPACING = (SPACING / 5);
	taskY = TEXTPIXY + y * SPACING + fifthSPACING;
	// taskW is the full width of the task from start to end
	taskW = daypix * project->tasks[taskno]->startdate->daysTo(project->tasks[taskno]->enddate->addDays(0));
	// taskWE is the full width of the task from start to expected end. End
	// can overrun the expected end.
	taskWE = daypix * project->tasks[taskno]->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
	// taskH is the height of the bars, and is three-fifths of the spacing
	taskH = fifthSPACING * 3;
	painter->setBrush(colorStr2Brush("lightblue"));
	painter->drawRect(taskX, taskY, taskWE, taskH);
	painter->setBrush(colorStr2Brush("red"));
	painter->drawRect(taskX + taskWE, taskY, taskW - taskWE, taskH);
	if (project->tasks[taskno]->completed > 0)
	{
		painter->setBrush(colorStr2Brush("blue"));
		painter->setPen("blue");
		// taskWcomp is the width that has been completed
		int taskWcomp = (taskW * project->tasks[taskno]->completed) / 100;
		painter->drawRect(taskX + 1, taskY + fifthSPACING, taskWcomp - 2, fifthSPACING);
	}
}


void GanttChart::paintmain(QPainter * painter, int taskno, int y)
{
	int xpos, ypos, delta;
	int fifthSPACING = (SPACING / 5);
	ypos = TEXTPIXY + y * SPACING + fifthSPACING;
	delta = SPACING / 4;
	QVector<QPoint> a;

	painter->setPen("black");
	if (project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->enddate->addDays(0)) != 0)
	{
		xpos = daypix * project->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
		int expenddays = project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
		int enddays = project->tasks[taskno]->expenddate->daysTo(project->tasks[taskno]->enddate->addDays(0));
		painter->setBrush(colorStr2Brush("red"));
		a =
		{
			{xpos + daypix * expenddays - delta, ypos},
			{xpos + daypix * enddays + delta, ypos},
			{xpos + daypix * enddays + delta, ypos + delta},
			{xpos + daypix * enddays, ypos + delta * 2},
			{xpos + daypix * enddays - delta, ypos + delta},
			{xpos + daypix * expenddays + delta, ypos + delta},
			{xpos + daypix * expenddays, ypos + delta * 2},
			{xpos + daypix * expenddays - delta,ypos + delta}
		};
		painter->drawPolygon(a);
	}

	painter->setBrush(colorStr2Brush("black"));
	int startdays = project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int expenddays = project->startdate->daysTo(project->tasks[taskno]->expenddate->addDays(0));
	a =
	{
		{daypix * startdays - delta, ypos},
		{daypix * expenddays + delta, ypos},
		{daypix * expenddays + delta, ypos + delta},
		{daypix * expenddays, ypos + delta * 2},
		{daypix * expenddays - delta, ypos + delta},
		{daypix * startdays + delta, ypos + delta},
		{daypix * startdays, ypos + delta * 2},
		{daypix * startdays - delta,ypos + delta}
	};
	painter->drawPolygon(a);
}


void GanttChart::paintmile(QPainter * painter, int taskno, int y)
{
	int xpos, ypos, delta;
	int fifthSPACING = (SPACING / 5);
	xpos = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	ypos = TEXTPIXY + y * SPACING + fifthSPACING;
	delta = SPACING / 4;
	painter->setPen("black");
	painter->setBrush(colorStr2Brush("black"));
	QVector<QPoint> a =
	{
		{xpos, ypos},
		{xpos + delta, ypos + delta},
		{xpos, ypos + delta * 2},
		{xpos - delta, ypos + delta}
	};

	painter->drawPolygon(a);
	QString qstr;
	painter->setFont(QFont("helvetica", FONTSIZE, QFont::Bold));
	painter->drawText(daypix * project->startdate->daysTo(
		project->tasks[taskno]->startdate->addDays(0)) + delta * 2,
					   ypos + delta * 2,
					   qstr.setNum(project->tasks[taskno]->startdate->day()
					   ) + "." + qstr.setNum(project->tasks[taskno]->startdate->month()));
}


void GanttChart::painttaskbeginarrow(QPainter * painter, int taskno, int y, int j)
{
	int xdelta = 7;
	int ydelta = 7;
	int start_j_end = daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0));
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	// ypos_j is the middle of the task box
	int ypos_j = TEXTPIXY + j * SPACING + (SPACING / 2);

	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < xdelta)
	{
		// dependency feeds back under the task
		// away from the task to the right
		painter->drawLine(start_j_end,
						   ypos_j,
						   start_j_end + xdelta,
						   ypos_j);
		// down to below the task
		painter->drawLine(start_j_end + xdelta,
						   ypos_j,
						   start_j_end + xdelta,
						   ypos_j + ydelta);
		// left under the task, to above the arrow
		painter->drawLine(start_j_end + xdelta,
						   ypos_j + ydelta,
						   start_taskno_start,
						   ypos_j + ydelta);
	}
	else
	{
		// dependency is "away from" the task
		// across..
		painter->drawLine(start_j_end,
						   ypos_j,
						   start_taskno_start,
						   ypos_j);
		// and down.
		painter->drawLine(start_taskno_start,
						   ypos_j,
						   start_taskno_start,
						   ypos_j + ydelta);
	}
}


void GanttChart::painttaskendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(colorStr2Brush("black"));
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int xdelta = 4;
	// ypos_j is the top of the box (not midpoint) of the task that is dependent
	// upon
	int ypos_j = TEXTPIXY + j * SPACING - 6;
	int ypos_y = TEXTPIXY + y * SPACING - 6;

	painter->drawLine(start_taskno_start,
					   ypos_j + 24,
					   start_taskno_start,
					   ypos_y + 8);
	QVector<QPoint> a =
	{
		{start_taskno_start - xdelta, ypos_y + 5},
		{start_taskno_start + xdelta, ypos_y + 5},
		{start_taskno_start, ypos_y + 9}
	};
	painter->drawPolygon(a);
}


void GanttChart::paintmainbeginarrow(QPainter * painter, int taskno, int y, int j)
{
	int start_j_end = daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0));
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int ypos = TEXTPIXY + j * SPACING;
	int xdelta = 7;
	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < xdelta)
	{
		painter->drawLine(start_j_end,
						   ypos + 26,
						   start_j_end,
						   ypos + 27);
		painter->drawLine(start_j_end,
						   ypos + 27,
						   start_taskno_start,
						   ypos + 27);
	}
	else
	{
		painter->drawLine(start_j_end,
						   ypos + 18,
						   start_taskno_start,
						   ypos + 18);
		painter->drawLine(start_taskno_start,
						   ypos + 18,
						   start_taskno_start,
						   ypos + 27);
	}
}


void GanttChart::paintmainendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(colorStr2Brush("black"));
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int ypos_y = TEXTPIXY + y * SPACING;
	int ypos_j = TEXTPIXY + j * SPACING;
	painter->drawLine(start_taskno_start,
					   ypos_j + 24,
					   start_taskno_start,
					   ypos_y + 8);
	QVector<QPoint> a =
	{
		{start_taskno_start, ypos_y + 5},
		{start_taskno_start, ypos_y + 5},
		{start_taskno_start, ypos_y + 9}
	};
	painter->drawPolygon(a);
}


void GanttChart::paintmilebeginarrow(QPainter * painter, int taskno, int y, int j)
{
	int start_j_end = daypix * project->startdate->daysTo(project->tasks[j]->enddate->addDays(0));
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int xdelta = 7;
	int ypos = TEXTPIXY + j * SPACING;
	if (project->tasks[j]->enddate->daysTo(project->tasks[taskno]->startdate->addDays(0)) * daypix < xdelta)
	{
		painter->drawLine(start_j_end,
						   ypos + 20,
						   start_j_end + xdelta,
						   ypos + 20);
		painter->drawLine(start_j_end + xdelta,
						   ypos + 20,
						   start_j_end + xdelta,
						   ypos + 27);
		painter->drawLine(start_j_end + xdelta,
						   ypos + 27,
						   start_taskno_start,
						   ypos + 27);
	}
	else
	{
		painter->drawLine(start_j_end,
						   ypos + 20,
						   start_taskno_start,
						   ypos + 20);
		painter->drawLine(start_taskno_start,
						   ypos + 20,
						   start_taskno_start,
						   ypos + 27);
	}
}


void GanttChart::paintmileendarrow(QPainter * painter, int taskno, int y, int j)
{
	painter->setPen("black");
	painter->setBrush(colorStr2Brush("black"));
	int xdelta = 4;
	int start_taskno_start = daypix * project->startdate->daysTo(project->tasks[taskno]->startdate->addDays(0));
	int ypos = TEXTPIXY + y * SPACING;
	int ypos_j = TEXTPIXY + j * SPACING;
	painter->drawLine(start_taskno_start,
					   ypos_j + 18,
					   start_taskno_start,
					   ypos + 2);
	QVector<QPoint> a =
	{
		{start_taskno_start - xdelta, ypos - 1},
		{start_taskno_start + xdelta, ypos - 1},
		{start_taskno_start, ypos + 3}
	};
	painter->drawPolygon(a);
}

