#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Connected.h"

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
	void on_axialSlider_valuechanged(int newValue);
	void on_coronalSlider_valuechanged(int newValue);
private:
	Ui::ConnectedClass ui;
};
