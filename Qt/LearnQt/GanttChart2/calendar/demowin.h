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

#ifndef __DEMOWIN_H
#define __DEMOWIN_H

#include <qwidget.h>

class DateInput;

class DemoWindow : public QWidget {
public:
  DemoWindow(QWidget*);
public:
  void slotDayClassify(int,int,char*);
  void slotDayLabel(int,int,int,char*);
private:
  DateInput* dateinput;
};

#endif

