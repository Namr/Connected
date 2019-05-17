#include "connected.h"
#define LOOKINGGLASS

Connected::Connected(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    KeyListener* key = new KeyListener(ui.screen);
    CSettings = new colorSettings();
    NSettings = new NetworkSettings();
    MSettings = new MriSettings();

    ui.centralWidget->installEventFilter(key);
    connect(ui.axialSlider, SIGNAL(valueChanged(int)), this, SLOT(on_axialSlider_valuechanged(int)));
    connect(ui.coronalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_coronalSlider_valuechanged(int)));

    ui.screen->nodeName = ui.nodeName;
    ui.screen->colors = CSettings->colors;

    ui.screen->nodeSize = &NSettings->nodeSize;
    ui.screen->isScaling = &NSettings->isScaling;
    ui.screen->connectionSize = &NSettings->connectionSize;
    ui.screen->graphSignalSize = &NSettings->graphSignalSize;
    ui.screen->threshold = &NSettings->threshold;
    ui.screen->textThreshold = &NSettings->textThreshold;
    ui.screen->textSize = &NSettings->textSize;
    ui.screen->displayFrame = &NSettings->displayFrame;
    ui.screen->msPerFrame = &NSettings->milisecsPerFrame;

    ui.screen->coronal = &MSettings->coronal;
    ui.screen->axial = &MSettings->axial;

    QTime dieTime = QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
#ifdef LOOKINGGLASS
    ui.verticalLayout_2->deleteLater();
    ui.menuBar->deleteLater();
#endif
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

void Connected::on_actionLoad_Primary_Brain_Mesh_triggered()
{
    QString meshName = QFileDialog::getOpenFileName(this, "Select Mesh File");

    if (!meshName.isEmpty() && !meshName.isNull())
    {
        ui.screen->primaryMeshName = meshName.toStdString();
        ui.screen->primaryShouldReload = 1;
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Secondary_Brain_Mesh_triggered()
{
    QString meshName = QFileDialog::getOpenFileName(this, "Select Mesh File");

    if (!meshName.isEmpty() && !meshName.isNull())
    {
        ui.screen->secondaryMeshName = meshName.toStdString();
        ui.screen->secondaryShouldReload = 1;
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Primary_Node_Data_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Graph Signal File");

    if (!nodeName.isEmpty() && !nodeName.isNull())
    {
        ui.screen->primaryBrain.loadAppendedNodeData(nodeName.toStdString());
        ui.screen->update();
    }
}

void Connected::on_actionLoad_Secondary_Node_Data_triggered()
{
    QString nodeName = QFileDialog::getOpenFileName(this, "Select Graph Signal File");

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

void Connected::on_actionColor_Settings_triggered()
{
    CSettings->show();
}

void Connected::on_actionRender_Settings_triggered()
{
    NSettings->show();
}

void Connected::on_actionMRI_Settings_triggered()
{
    MSettings->show();
}

void Connected::on_actionSave_Settings_triggered()
{
    QString settingsFile = QFileDialog::getSaveFileName(this, "Select Project File");
    QSettings settings(settingsFile, QSettings::IniFormat);

    //loop through colors
    for(int i = 0; i < 13; i++)
    {
        //loop through color components
        for(int c = 0; c < 3; c++)
        {
            std::string name = "Color" + std::to_string(i) + ":" + std::to_string(c);
            if(c == 0)
                settings.setValue(QString(name.c_str()), CSettings->colors[i].R);
            else if(c == 1)
                settings.setValue(QString(name.c_str()), CSettings->colors[i].G);
            else
                settings.setValue(QString(name.c_str()), CSettings->colors[i].B);
        }
    }

    settings.setValue("nodeSize", NSettings->nodeSize);
    settings.setValue("isScaling", NSettings->isScaling);
    settings.setValue("connectionSize", NSettings->connectionSize);
    settings.setValue("graphSignalSize", NSettings->graphSignalSize);
    settings.setValue("threshold", NSettings->threshold);
    settings.setValue("textThreshold", NSettings->textThreshold);
    settings.setValue("textSize", NSettings->textSize);
    settings.setValue("displayFrame", NSettings->displayFrame);
    settings.setValue("msPerFrame", NSettings->milisecsPerFrame);

    //unpack MTransform and put its components into

    for(int i = 0; i < 6; i++)
    {
        std::string name = "Transform0:" + std::to_string(i);
        switch(i)
        {
            case 0:
                settings.setValue(QString(name.c_str()), MSettings->coronal.x);
                break;
            case 1:
                settings.setValue(QString(name.c_str()), MSettings->coronal.y);
                break;
            case 2:
                settings.setValue(QString(name.c_str()), MSettings->coronal.z);
                break;
            case 3:
                settings.setValue(QString(name.c_str()), MSettings->coronal.sx);
                break;
            case 4:
                settings.setValue(QString(name.c_str()), MSettings->coronal.sy);
                break;
            case 5:
                settings.setValue(QString(name.c_str()), MSettings->coronal.sz);
                break;
        }
    }

    for(int i = 0; i < 6; i++)
    {
        std::string name = "Transform1:" + std::to_string(i);
        switch(i)
        {
            case 0:
                settings.setValue(QString(name.c_str()), MSettings->axial.x);
                break;
            case 1:
                settings.setValue(QString(name.c_str()), MSettings->axial.y);
                break;
            case 2:
                settings.setValue(QString(name.c_str()), MSettings->axial.z);
                break;
            case 3:
                settings.setValue(QString(name.c_str()), MSettings->axial.sx);
                break;
            case 4:
                settings.setValue(QString(name.c_str()), MSettings->axial.sy);
                break;
            case 5:
                settings.setValue(QString(name.c_str()), MSettings->axial.sz);
                break;
        }
    }

    settings.sync();
}

void Connected::on_actionLoad_Project_triggered()
{
    QString settingsFile = QFileDialog::getOpenFileName(this, "Select Project File");
    QSettings settings(settingsFile, QSettings::IniFormat);

    //loop through colors
    for(int i = 0; i < 13; i++)
    {
        //loop through color components
        for(int c = 0; c < 3; c++)
        {
            std::string name = "Color" + std::to_string(i) + ":" + std::to_string(c);
            if(c == 0)
                CSettings->colors[i].R = settings.value(QString(name.c_str()), 0).toInt();
            else if(c == 1)
                CSettings->colors[i].G = settings.value(QString(name.c_str()), 0).toInt();
            else
                CSettings->colors[i].B = settings.value(QString(name.c_str()), 0).toInt();
        }
    }

    ui.screen->colors = CSettings->colors;
    CSettings->refresh();

    NSettings->nodeSize = settings.value("nodeSize", 1.0).toFloat();
    NSettings->isScaling = settings.value("isScaling", 0.0).toBool();
    NSettings->connectionSize = settings.value("connectionSize", 1.0).toFloat();
    NSettings->graphSignalSize = settings.value("graphSignalSize", 1.0).toFloat();
    NSettings->threshold = settings.value("threshold", 1.0).toFloat();
    NSettings->textThreshold = settings.value("textThreshold", 1.0).toFloat();
    NSettings->textSize = settings.value("textSize", 1.0).toInt();
    NSettings->displayFrame = settings.value("displayFrame", 0.0).toBool();
    NSettings->milisecsPerFrame = settings.value("msPerFrame", 220).toInt();
    NSettings->refresh();

    //unpack MTransform and put its components into

    for(int i = 0; i < 6; i++)
    {
        std::string name = "Transform0:" + std::to_string(i);
        switch(i)
        {
            case 0:
                MSettings->coronal.x = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 1:
                MSettings->coronal.y = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 2:
                MSettings->coronal.z = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 3:
                MSettings->coronal.sx = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 4:
                MSettings->coronal.sy = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 5:
                MSettings->coronal.sz = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
        }
    }

    for(int i = 0; i < 6; i++)
    {
        std::string name = "Transform1:" + std::to_string(i);
        switch(i)
        {
            case 0:
                MSettings->axial.x = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 1:
                MSettings->axial.y = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 2:
                MSettings->axial.z = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 3:
                MSettings->axial.sx = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 4:
                MSettings->axial.sy = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
            case 5:
                MSettings->axial.sz = settings.value(QString(name.c_str()), 1.0).toInt();
                break;
        }
    }

    MSettings->refresh();
    settings.sync();
}
