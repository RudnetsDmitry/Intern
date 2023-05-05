#include "stdafx.h"
#include "ExportFunctions.h"
#include "mainwidget.h"

namespace gantt_chart2
{
    QMainWindow * MakeChart()
    {
        return new MainWidget("c:/Projects/Intern/Qt/LearnQt/GanttChart2/test.gantt");
    }
}

