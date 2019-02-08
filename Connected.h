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
private:
	Ui::ConnectedClass ui;
};
