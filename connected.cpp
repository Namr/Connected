#include "connected.h"

Connected::Connected(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    KeyListener* key = new KeyListener(ui.screen);
    ui.centralWidget->installEventFilter(key);
    connect(ui.axialSlider, SIGNAL(valueChanged(int)), this, SLOT(on_axialSlider_valuechanged(int)));
    connect(ui.coronalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_coronalSlider_valuechanged(int)));
    connect(ui.thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(on_thresholdSlider_valuechanged(int)));
    ui.screen->nodeName = ui.nodeName;

    QTime dieTime = QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}

void Connected::on_actionLoad_Connectome_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Node File");
    QString edgeName = QFileDialog::getOpenFileName(this, "Select Edge File");

    if (!nodeName.isEmpty() && !nodeName.isNull() && !edgeName.isEmpty() && !edgeName.isNull())
    {
        ui.screen->primaryNodeName = nodeName.toStdString().c_str();
        ui.screen->primaryEdgeName = edgeName.toStdString().c_str();
        ui.screen->primaryShouldReload = 1;
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Secondary_Connectome_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Node File");
    QString edgeName = QFileDialog::getOpenFileName(this, "Select Edge File");

    if (!nodeName.isEmpty() && !nodeName.isNull() && !edgeName.isEmpty() && !edgeName.isNull())
    {
        ui.screen->secondaryNodeName = nodeName.toStdString().c_str();
        ui.screen->secondaryEdgeName = edgeName.toStdString().c_str();
        ui.screen->secondaryShouldReload = 1;
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Primary_Node_Data_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Node Data File");

    if (!nodeName.isEmpty() && !nodeName.isNull())
    {
        ui.screen->primaryBrain.loadAppendedNodeData(nodeName.toStdString());
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Secondary_Node_Data_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Node Data File");

    if (!nodeName.isEmpty() && !nodeName.isNull())
    {
        ui.screen->secondaryBrain.loadAppendedNodeData(nodeName.toStdString());
        ui.screen->update();
    }
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

void Connected::on_thresholdSlider_valuechanged(int newValue)
{
    ui.screen->threshold = (float)newValue / 100.0f;
    ui.screen->update();
}
