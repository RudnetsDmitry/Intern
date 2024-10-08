/// @file	MainWindow.cpp
///
/// @see	MainWindow.h
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 23:01
///

#include "stdafx.h"
#include "MainWindow.h"

#include "./tmp/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
