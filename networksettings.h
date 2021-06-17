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
    bool isConnectionScaling = true;
    bool displayFrame = false;
    bool displayHeatmap = false;
    bool connectionStrengthColor = true;
    bool showUnusedNodes = true;
    int milisecsPerFrame = 220;
private slots:
    void on_nodeSizeSlider_sliderMoved(int position);

    void on_ConnectionSizeSlider_sliderMoved(int position);

    void on_GraphSignalSizeSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_checkBox_toggled(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_horizontalSlider_4_sliderMoved(int position);

    void on_checkBox_3_toggled(bool checked);

    void on_checkBox_4_toggled(bool checked);

    void on_checkBox_5_toggled(bool checked);

    void on_checkBox_6_toggled(bool checked);
private:
    Ui::NetworkSettings *ui;
};

#endif // NETWORKSETTINGS_H
