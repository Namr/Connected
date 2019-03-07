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
