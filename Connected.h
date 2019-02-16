#pragma once

#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include "ui_Connected.h"
#include "KeyListener.h"
#include "Brain.h"

class Connected : public QMainWindow
{
	Q_OBJECT

public:
	Connected(QWidget *parent = Q_NULLPTR);

private slots:
	void on_singleview_clicked();
	void on_multiview_clicked();
	void on_compareview_clicked();
	void on_meshenable_clicked();
	void on_mrienable_clicked();
	void on_actionLoad_Connectome_triggered();
	void on_actionLoad_Secondary_Connectome_triggered();
	void on_axialSlider_valuechanged(int newValue);
	void on_coronalSlider_valuechanged(int newValue);
private:
	Ui::ConnectedClass ui;
};
