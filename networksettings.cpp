#include "networksettings.h"
#include "ui_networksettings.h"

NetworkSettings::NetworkSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetworkSettings)
{
    ui->setupUi(this);
}

NetworkSettings::~NetworkSettings()
{
    delete ui;
}

void NetworkSettings::refresh()
{
    ui->nodeSizeSlider->setValue((int) nodeSize * 100);
    ui->ConnectionSizeSlider->setValue((int) connectionSize * 100);
    ui->GraphSignalSizeSlider->setValue((int) graphSignalSize);
    ui->horizontalSlider->setValue((int) threshold * 100);
    ui->horizontalSlider_2->setValue((int)textThreshold * 100);
    ui->horizontalSlider_3->setValue(textSize);
}

void NetworkSettings::on_nodeSizeSlider_sliderMoved(int position)
{
    nodeSize = position / 100.0f;
}

void NetworkSettings::on_ConnectionSizeSlider_sliderMoved(int position)
{
    connectionSize = position / 100.0f;
}

void NetworkSettings::on_GraphSignalSizeSlider_sliderMoved(int position)
{
    graphSignalSize = position;
}

void NetworkSettings::on_horizontalSlider_sliderMoved(int position)
{
    threshold = position / 100.0f;
}

void NetworkSettings::on_horizontalSlider_2_sliderMoved(int position)
{
    textThreshold = position / 100.0f;
}

void NetworkSettings::on_horizontalSlider_3_sliderMoved(int position)
{
    textSize = position;
}
