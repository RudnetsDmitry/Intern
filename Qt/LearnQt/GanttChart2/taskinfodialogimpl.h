//===========================================================================
//
//  Filename  : taskinfodialogimpl.h
//  Purpose   : Task Information Dialog Implementation Definition
//  Copyright : Copyright (C) 2000 Matt J. Gumbley <matt@gumbley.demon.co.uk>
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

#ifndef TASKINFODIALOGIMPL_H
#define TASKINFODIALOGIMPL_H

#include "taskinfodialog.h"
#include "task.h"


class TaskInfoDialogImpl : public TaskInfoDialog
{ 
public:
	enum tidview { GENERAL, PREDECESSORS, NOTES };
	TaskInfoDialogImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
	~TaskInfoDialogImpl();
	void SetView( tidview v );
	void SetTask( Task * );
public:
	virtual void apply() override;
private:
	tidview view;
	Task *task;
};

#endif // TASKINFODIALOGIMPL_H