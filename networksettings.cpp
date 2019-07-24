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
    ui->horizontalSlider_4->setValue(milisecsPerFrame);

    if(isScaling)
        ui->checkBox->setCheckState(Qt::CheckState::Checked);
    else
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);

    if(displayFrame)
        ui->checkBox_2->setCheckState(Qt::CheckState::Checked);
    else
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
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

void NetworkSettings::on_checkBox_toggled(bool checked)
{
    isScaling = checked;
}

void NetworkSettings::on_checkBox_2_clicked(bool checked)
{
    displayFrame = checked;
}

void NetworkSettings::on_horizontalSlider_4_sliderMoved(int position)
{
    milisecsPerFrame = position;
}

void NetworkSettings::on_checkBox_3_toggled(bool checked)
{
    displayHeatmap = checked;
}

void NetworkSettings::on_checkBox_4_toggled(bool checked)
{
    connectionStrengthColor = checked;
}
