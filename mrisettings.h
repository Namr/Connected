#ifndef MRISETTINGS_H
#define MRISETTINGS_H

#include <QMainWindow>
#include "mri.h"

namespace Ui {
class MriSettings;
}

class MriSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit MriSettings(QWidget *parent = nullptr);
    ~MriSettings();

    void refresh();

    MTransform coronal;
    MTransform axial;
private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_XPos_valueChanged(int arg1);

    void on_YPos_valueChanged(int arg1);

    void on_ZPos_valueChanged(int arg1);

    void on_XScale_valueChanged(int arg1);

    void on_YScale_valueChanged(int arg1);

    void on_ZScale_valueChanged(int arg1);

private:
    Ui::MriSettings *ui;
    int curSlice = 0; //indicates which of the orientations of MRI is selected 0 for coronal, 1 for axial
};

#endif // MRISETTINGS_H
