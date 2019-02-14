#include "Connected.h"

Connected::Connected(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	KeyListener* key = new KeyListener(ui.screen);
	ui.centralWidget->installEventFilter(key);
	connect(ui.axialSlider, SIGNAL(valueChanged(int)), this, SLOT(on_axialSlider_valuechanged(int))); 
	connect(ui.coronalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_coronalSlider_valuechanged(int)));
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

void Connected::on_meshenable_clicked()
{
	ui.screen->primaryBrain.displayShell *= -1;
	ui.screen->secondaryBrain.displayShell *= -1;
	ui.screen->update();
}

void Connected::on_mrienable_clicked()
{
	ui.screen->primaryBrain.displayMri *= -1;
	ui.screen->secondaryBrain.displayMri *= -1;
	ui.screen->update();
}

void Connected::on_axialSlider_valuechanged(int newValue)
{
	ui.screen->primaryBrain.mri.axialPosition = newValue;
	ui.screen->secondaryBrain.mri.axialPosition = newValue;
	ui.screen->update();
}

void Connected::on_coronalSlider_valuechanged(int newValue)
{
	ui.screen->primaryBrain.mri.coronalPosition = newValue;
	ui.screen->secondaryBrain.mri.coronalPosition = newValue;
	ui.screen->update();
}
