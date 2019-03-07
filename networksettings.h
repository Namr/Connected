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

private slots:
    void on_nodeSizeSlider_sliderMoved(int position);

private:
    Ui::NetworkSettings *ui;
};

#endif // NETWORKSETTINGS_H
