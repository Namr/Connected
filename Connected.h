#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Connected.h"

class Connected : public QMainWindow
{
	Q_OBJECT

public:
	Connected(QWidget *parent = Q_NULLPTR);

private:
	Ui::ConnectedClass ui;
};
