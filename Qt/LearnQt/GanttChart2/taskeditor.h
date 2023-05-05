//===========================================================================
//
//  Filename  : taskeditor.h
//  Purpose   : Task Editor Widget for QtGantt
//  Copyright : Copyright (C) 2000 Olivier Berrod <olivier.berrod@free.fr>
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


#ifndef TASKEDITOR_H
#define TASKEDITOR_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qlayout.h>

#include "task.h"

class QDate;
class QString;
class QListView;
class QSplitter;
class QWidget;
class Project;
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;



class TaskField : public QWidget
{
public:
	TaskField( QWidget *parent=0);

	void RefreshValues();
	void SetTask(Task  *task=0);
	void RefreshComBox_type();

public:
	void change_description(const QString& item);
	void change_startdate(const QString& item);
	//void change_startdate();
	void change_enddate(const QString& item);

protected:
	void paintEvent( QPaintEvent * );
private:
	Task* LocalTask;

	QLineEdit* LineEdit_description;
	QLineEdit* LineEdit_start_date;
	QLineEdit* LineEdit_stop_date;
	QComboBox* ComboBox_type;

	QLineEdit* LineEdit10;
	QLineEdit* LineEdit11;
	QSpinBox* SpinBox2;

	QVBoxLayout* layout1;
	QVBoxLayout* layout2;

	QPushButton* PushButton_update;
};


#endif  // TASKEDITOR_H


