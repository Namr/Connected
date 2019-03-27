#ifndef CONNECTED_H
#define CONNECTED_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <QTime>
#include <QSettings>

#include "ui_connected.h"
#include "colorsettings.h"
#include "networksettings.h"
#include "mrisettings.h"
#include "keylistener.h"
#include "brain.h"

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
    void on_actionLoad_Primary_Node_Data_triggered();
    void on_actionLoad_Secondary_Node_Data_triggered();
    void on_axialSlider_valuechanged(int newValue);
    void on_coronalSlider_valuechanged(int newValue);

    void on_actionColor_Settings_triggered();

    void on_actionRender_Settings_triggered();

    void on_actionMRI_Settings_triggered();

    void on_actionSave_Settings_triggered();

    void on_actionLoad_Project_triggered();

    void on_actionLoad_Primary_Brain_Mesh_triggered();

    void on_actionLoad_Secondary_Brain_Mesh_triggered();

private:
    Ui::ConnectedClass ui;
    colorSettings *CSettings;
    NetworkSettings *NSettings;
    MriSettings *MSettings;
};

#endif // CONNECTED_H
