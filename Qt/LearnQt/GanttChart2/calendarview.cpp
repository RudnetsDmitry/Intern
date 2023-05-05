//===========================================================================
//
//  Filename  : calendarview.cpp
//  Purpose   : Calendar View Widget for QtGantt
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

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>

#include "project.h"
#include "calendarview.h"

CalendarView::CalendarView ( Project *proj, QWidget *parent, const char *name )
    : QWidget( parent/*, name */)
{
	project = proj;
	//setBackgroundColor( "blue" );
}

CalendarView::~CalendarView ()
{
}

void CalendarView::paint( QPainter *painter )
{
  painter->setPen( "black" );
  painter->drawLine(0,0, 10, 10);
}

