#include "Connected.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Connected w;
	w.show();
	return a.exec();
}
