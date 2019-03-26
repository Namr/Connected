#include "colorsettings.h"
#include "ui_colorsettings.h"

colorSettings::colorSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::colorSettings)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    colors[0].R = 0;
    colors[0].G = 0;
    colors[0].B = 255;

    colors[1].R = 0;
    colors[1].G = 255;
    colors[1].B = 0;

    colors[2].R = 255;
    colors[2].G = 255;
    colors[2].B = 0;

    colors[3].R = 255;
    colors[3].G = 0;
    colors[3].B = 85;

    colors[4].R = 0;
    colors[4].G = 255;
    colors[4].B = 255;

    colors[5].R = 255;
    colors[5].G = 127;
    colors[5].B = 0;

    colors[6].R = 255;
    colors[6].G = 127;
    colors[6].B = 127;

    colors[6].R = 127;
    colors[6].G = 127;
    colors[6].B = 255;

    colors[7].R = 127;
    colors[7].G = 255;
    colors[7].B = 127;

    colors[8].R = 127;
    colors[8].G = 255;
    colors[8].B = 127;

    colors[9].R = 127;
    colors[9].G = 0;
    colors[9].B = 255;

    colors[10].R = 0;
    colors[10].G = 0;
    colors[10].B = 0;

    colors[11].R = 230;
    colors[11].G = 230;
    colors[11].B = 230;
    colors[11].A = 69;

    colors[12].R = 255;
    colors[12].G = 255;
    colors[12].B = 0;

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}

colorSettings::~colorSettings()
{
    delete ui;
}

//red
void colorSettings::on_horizontalSlider_sliderMoved(int position)
{
    colors[selectedColor].R = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}

//green
void colorSettings::on_horizontalSlider_2_sliderMoved(int position)
{
    colors[selectedColor].G = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}
//blue
void colorSettings::on_horizontalSlider_3_sliderMoved(int position)
{
    colors[selectedColor].B = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}

void colorSettings::on_horizontalSlider_4_sliderMoved(int position)
{
    colors[selectedColor].A = position;
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}

//node 1
void colorSettings::on_radioButton_clicked()
{
    selectedColor = 0;
    refresh();
}

//node 2
void colorSettings::on_radioButton_2_clicked()
{
    selectedColor = 1;
    refresh();
}

//node 3
void colorSettings::on_radioButton_3_clicked()
{
    selectedColor = 2;
    refresh();
}

//node 4
void colorSettings::on_radioButton_4_clicked()
{
    selectedColor = 3;
    refresh();
}

//node 5
void colorSettings::on_radioButton_5_clicked()
{
    selectedColor = 4;
    refresh();
}

//node 6
void colorSettings::on_radioButton_6_clicked()
{
    selectedColor = 5;
    refresh();
}

//node 7
void colorSettings::on_radioButton_10_clicked()
{
    selectedColor = 6;
    refresh();
}

//node 8
void colorSettings::on_radioButton_11_clicked()
{
    selectedColor = 7;
    refresh();
}

//node 9
void colorSettings::on_radioButton_12_clicked()
{
    selectedColor = 8;
    refresh();
}

//node 10
void colorSettings::on_radioButton_13_clicked()
{
    selectedColor = 9;
    refresh();
}

//background
void colorSettings::on_radioButton_7_clicked()
{
    selectedColor = 10;
    refresh();
}

//brain mesh
void colorSettings::on_radioButton_8_clicked()
{
    selectedColor = 11;
    refresh();
}

//text
void colorSettings::on_radioButton_9_clicked()
{
    selectedColor = 12;
    refresh();
}

void colorSettings::refresh()
{
    ui->horizontalSlider->setValue(colors[selectedColor].R);
    ui->horizontalSlider_2->setValue(colors[selectedColor].G);
    ui->horizontalSlider_3->setValue(colors[selectedColor].B);
    ui->horizontalSlider_4->setValue(colors[selectedColor].A);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(colors[selectedColor].R, colors[selectedColor].G, colors[selectedColor].B, colors[selectedColor].A), Qt::SolidPattern));
}
