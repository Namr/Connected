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
    float nodeSize = 1.5f;
    float connectionSize = 0.3f;
    float graphSignalSize = 20.0f;
private slots:
    void on_nodeSizeSlider_sliderMoved(int position);

    void on_ConnectionSizeSlider_sliderMoved(int position);

    void on_GraphSignalSizeSlider_sliderMoved(int position);

private:
    Ui::NetworkSettings *ui;
};

#endif // NETWORKSETTINGS_H
