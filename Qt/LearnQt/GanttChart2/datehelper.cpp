//===========================================================================
//
//  Filename  : datehelper.cpp
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
#include "stdafx.h"
#include <qapplication.h>
#include <qstring.h>
#include <qdatetime.h>

#include <QtCore/QString>

#include "datehelper.h"

void DateHelper::String2Date(QString *local_string,QDate *export_date)
{
	qDebug("String2Date");

	int position, position_end;
	int day;
	int month;
	int year;
	QString *temp_string_date;
	QString *temp_date;
	QDate   *valid_date;

	temp_date =new QString(*local_string);
	//qDebug( *temp_date );
	temp_string_date =new  QString(*local_string);
	valid_date = new QDate();

	//extract days
	position=temp_date->indexOf("/",0);
	//qDebug( "%d\n", position );
	*temp_string_date=temp_date->left(position  );
	day = temp_string_date->toInt();
	//qDebug("day :%d \n",day);

	//extract month
	position_end=temp_date->lastIndexOf("/",-1);
	*temp_string_date=temp_date->mid(position+1,
			(position_end-position)-1 );
	//qDebug(*temp_string_date);
	month = temp_string_date->toInt();
	//qDebug("month :%d \n",month);

	//extract year
	*temp_string_date=temp_date->right(temp_date->length()-
			(position_end)-1 );
	//qDebug(*temp_string_date);
	year = temp_string_date->toInt();
	//qDebug("year :%d \n",year);    

	valid_date->setDate(year,month,day);

	*export_date = *valid_date;

	if (valid_date)
		delete valid_date;
	if (temp_date)
		delete temp_date;
	if (temp_string_date)
		delete temp_string_date;
}


// another class method
QString DateHelper::Date2String(const QDate &date)
{
	return QString("%1-%2-%3")
		.arg(date.day(), 2, 10, QChar('0'))
		.arg(date.month(), 2, 10, QChar('0'))
		.arg(date.year(), 4, 10, QChar('0'));
}

