#ifndef COLORSETTINGS_H
#define COLORSETTINGS_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class colorSettings;
}

struct NColor
{
    int R = 0;
    int G = 0;
    int B = 0;
    int A = 255;
};

class colorSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit colorSettings(QWidget *parent = nullptr);
    ~colorSettings();

    void refresh();

    NColor colors[9];
private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_8_clicked();

    void on_radioButton_9_clicked();

    void on_horizontalSlider_4_sliderMoved(int position);

private:
    Ui::colorSettings *ui;
    QGraphicsScene *scene;
    int selectedColor = 0;
};

#endif // COLORSETTINGS_H
