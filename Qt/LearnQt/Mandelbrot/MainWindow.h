/// @file	MainWindow.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 22:58
///

#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   // Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

