//===========================================================================
//
//  Filename  : projectinfodialogimpl.cpp
//  Purpose   : Project Information Dialog Implementation
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
#include "stdafx.h"
#include <qlineedit.h>
//#include "calendar/calendar.h"

#include "projectinfodialogimpl.h"
#include "project.h"
#include <QtCore/QDateTime>

/*
 *  Constructs a ProjectInfoDialogImpl which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectInfoDialogImpl::ProjectInfoDialogImpl( QWidget* parent,  Project* proj)
    : ProjectInfoDialog( parent)
{
	// Save for later in the dialog
	project = proj;

	// Populate dialog
	QString name;
	project->GetName( &name );
	Title->setText( name );

	QDate date = project->startdate->addDays(0);
	//StartDate->setDate( date );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ProjectInfoDialogImpl::~ProjectInfoDialogImpl()
{
    // no need to delete child widgets, Qt does it all for us
}

void ProjectInfoDialogImpl::apply()
{
	save();
}

void ProjectInfoDialogImpl::accept()
{
	save();
	ProjectInfoDialog::accept();
}

void ProjectInfoDialogImpl::reject()
{
	ProjectInfoDialog::reject();
}

void ProjectInfoDialogImpl::save()
{
	QString name = Title->text();
	project->SetName( &name );
	//QDate date = StartDate->getQDate();
	//project->SetStartDate( &date );
}

