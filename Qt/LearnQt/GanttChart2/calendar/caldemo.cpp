/*
 *
 * Copyright (c) 1998,1999, Mark Jackson.  
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as included in
 * the file COPYING in the main directory for this package.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include "stdafx.h"
#include <stdio.h>

//#include <qapp.h>

#include "calendar.h"
#include "demowin.h"
#include "worktime.h"

DemoWindow::DemoWindow(QWidget* p)
:QWidget(p)
{
  dateinput=new DateInput(this,"5/25/1999");
  QWidget::connect(dateinput,SIGNAL(classifyRequest(int,int,char*)),
	this,SLOT(slotDayClassify(int,int,char*)));
  connect(dateinput,SIGNAL(dayLabel(int,int,int,char*)),
	this,SLOT(slotDayLabel(int,int,int,char*)));
  dateinput->setGeometry(10,10,130,25);
  dateinput->show();
}

void DemoWindow::slotDayClassify(int m, int y, char* dc)
{
  WorkTime wt(y);
  for (int i=0; i<wt.monthLength(m); i++) {
    switch (wt.dayType(m,i)) {
    case 0:
    default:
      dc[i]=NoColor;
      break;
    case 1:
      dc[i]=Color1;
      break;
    case 2:
      dc[i]=Color2;
      break;
    case 3:
      dc[i]=Color2;
    }
  }
}

void DemoWindow::slotDayLabel(int m, int d, int y, char* dl)
{
  WorkTime wt(y);
  wt.numberWorkDays();
  if (wt.isWorkDay(m,d-1))
    sprintf(dl,"Work day #%i of %i",wt.dayNumber(m,d-1),y);
  else if (wt.isHoliday(m,d-1))
    sprintf(dl,"%s",wt.dayText(m,d-1));
  else if (wt.isWeekend(m,d-1))
    sprintf(dl,"Weekend");
}

//int main(int argc, char** argv)
//{
//  QApplication app(argc, argv);
//  DemoWindow mainw(NULL);
//  mainw.setMinimumSize(300,300);
//  mainw.show();
//  app.setMainWidget(&mainw);
//  return app.exec();
//}

