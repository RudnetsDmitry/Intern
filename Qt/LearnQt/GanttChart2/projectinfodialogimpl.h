//===========================================================================
//
//  Filename  : projectinfodialogimpl.h
//  Purpose   : Project Information Dialog Implementation Declaration
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

#ifndef PROJECTINFODIALOGIMPL_H
#define PROJECTINFODIALOGIMPL_H
#include "projectinfodialog.h"

class Project;

class ProjectInfoDialogImpl : public ProjectInfoDialog
{
public:
    ProjectInfoDialogImpl( QWidget* parent = 0, Project * proj = 0);
    ~ProjectInfoDialogImpl();

public:
    virtual void accept() override;
    virtual void reject() override;
    virtual void apply() override;

private:
    Project * project;
    void save();
};

#endif // PROJECTINFODIALOGIMPL_H
