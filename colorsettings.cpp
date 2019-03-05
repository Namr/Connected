#include "colorsettings.h"
#include "ui_colorsettings.h"

colorSettings::colorSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::colorSettings)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    colors[selectedColor].R = 255;
    colors[selectedColor].G = 255;
    colors[selectedColor].B = 255;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

colorSettings::~colorSettings()
{
    delete ui;
}

//red
void colorSettings::on_horizontalSlider_sliderMoved(int position)
{
    colors[selectedColor].R = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

//green
void colorSettings::on_horizontalSlider_2_sliderMoved(int position)
{
    colors[selectedColor].G = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}
//blue
void colorSettings::on_horizontalSlider_3_sliderMoved(int position)
{
    colors[selectedColor].B = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_clicked()
{
    selectedColor = 0;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_2_clicked()
{
    selectedColor = 1;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_3_clicked()
{
    selectedColor = 2;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_4_clicked()
{
    selectedColor = 3;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_5_clicked()
{
    selectedColor = 4;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}


void colorSettings::on_radioButton_6_clicked()
{
    selectedColor = 5;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}

void colorSettings::on_radioButton_7_clicked()
{
    selectedColor = 6;
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B), Qt::SolidPattern));
}
