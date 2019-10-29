
#include "MainWindow.h"
#include <QtWidgets\QApplication>

#include "..\GaleryGUI\ExportFunctions.h"
//#include <winnt.h>
//#include <winnls.h>

int main(int argc, char * argv[])
{
	//::SetThreadLocale(MAKESORTLCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication app(argc, argv);

	QMainWindow * mainWnd = ::gallery::MakeGalleryWindow();
	if (!mainWnd)
		mainWnd = new qt_learn::MainWindow();
	mainWnd->show();

	return app.exec();
}
