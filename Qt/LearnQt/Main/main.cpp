
#include "MainWindow.h"
#include <QtWidgets\QApplication>
//#include <winnt.h>
//#include <winnls.h>

int main(int argc, char * argv[])
{
	//::SetThreadLocale(MAKESORTLCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT), SORT_DEFAULT));
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication app(argc, argv);
	
	qt_learn::MainWindow w;
	w.show();

	return app.exec();
}
