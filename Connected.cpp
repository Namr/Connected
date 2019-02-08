#include "Connected.h"

Connected::Connected(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Connected::on_multiview_clicked()
{
	ui.screen->viewingMode = 1;
	ui.screen->update();
}


void Connected::on_singleview_clicked()
{
	ui.screen->viewingMode = 2;
	ui.screen->update();
}


void Connected::on_compareview_clicked()
{
	ui.screen->viewingMode = 3;
	ui.screen->update();
}

