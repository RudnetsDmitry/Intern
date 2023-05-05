﻿//===========================================================================
//
//  Filename  : optionsdialogimpl.cpp
//  Purpose   : Options Dialog Implementation
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
#include "optionsdialogimpl.h"

/* 
 *  Constructs a OptionsDialogImpl which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OptionsDialogImpl::OptionsDialogImpl( QWidget* parent)
    : OptionsDialog( parent)
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
OptionsDialogImpl::~OptionsDialogImpl()
{
    // no need to delete child widgets, Qt does it all for us
}

