
#include "MainWindow.h"
#include <QtWidgets\QApplication>

#include "..\GaleryGUI\ExportFunctions.h"
#include "..\QmlGUI\ExportFunctions.h"
#include "..\GanttChart\ExportFunctions.h"
#include "..\GanttChart2\ExportFunctions.h"
//#include <winnt.h>
//#include <winnls.h>

namespace
{
	class TestOperation
	{
		int value;
	public:
	};
}

int main(int argc, char * argv[])
{
	//::SetThreadLocale(MAKESORTLCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
	QApplication app(argc, argv);

	int ii = sizeof(std::wstring);
	int ii1 = sizeof(QString);
	
	QMainWindow* mainWnd = gantt_chart2::MakeChart();
	if (!mainWnd)
		mainWnd = gantt_chart::MakeChart();
	if (!mainWnd)
		mainWnd = ::qml_gui::MakeQmlWindow();
	if (!mainWnd)
		mainWnd = ::gallery::MakeGalleryWindow();
	if (!mainWnd)
		mainWnd = new qt_learn::MainWindow();
	mainWnd->show();

	return app.exec();
}
