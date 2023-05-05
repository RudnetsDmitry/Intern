//===========================================================================
//
//  Filename  : datehelper.h
//  Purpose   : Helper functions for dates
//  Copyright : Copyright (C) 2000 Matt J. Gumbley <matt@gumbley.demon.co.uk>
//  Original  : by Olivier Berrod
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

#ifndef DATEHELPER_H
#define DATEHELPER_H

#include <qdatetime.h>
#include <qstring.h>

class DateHelper
{
	public:
		static void String2Date(QString *local_string,QDate *export_date);
		static QString Date2String(const QDate &date);
};


#endif // DATEHELPER_H
