#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QMainWindow>

namespace Ui {
class NetworkSettings;
}

class NetworkSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetworkSettings(QWidget *parent = nullptr);
    ~NetworkSettings();
    void refresh();

    float nodeSize = 1.5f;
    float connectionSize = 0.3f;
    float graphSignalSize = 20.0f;
    float threshold = 0.5f;
    float textThreshold = 1.0f;
    int textSize = 10;
    bool isScaling = false;
private slots:
    void on_nodeSizeSlider_sliderMoved(int position);

    void on_ConnectionSizeSlider_sliderMoved(int position);

    void on_GraphSignalSizeSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_checkBox_toggled(bool checked);

private:
    Ui::NetworkSettings *ui;
};

#endif // NETWORKSETTINGS_H
