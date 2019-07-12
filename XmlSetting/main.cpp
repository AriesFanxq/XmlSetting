#include "xmlsetting.h"
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;
	w.setMinimumSize(800, 600);
	//¾çÖÐÏÔÊ¾
	w.move((a.desktop()->width() - w.width()) / 2, (a.desktop()->height() - w.height()) / 2);
	w.show();

	return a.exec();
}
