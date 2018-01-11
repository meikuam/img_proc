#ifndef BRIGHTNESSWINDOW_H
#define BRIGHTNESSWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "imgdata.h"

namespace Ui {
class BrightnessWindow;
}

class BrightnessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrightnessWindow(ImgData *data, QListWidget* listWidget, QWidget *parent = 0 );
    ~BrightnessWindow();
    ImgData *img,
            *local;
    int value;
    bool channels[3];

    Q_SIGNAL
    void repaint();
    Q_SIGNAL
    void setImage(QImage* img);

private slots:

    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_valueSlider_valueChanged(int value);
    void on_valueEdit_textChanged(const QString &arg1);
    void changeImage();

    void on_channelBox_currentIndexChanged(int index);

private:
    Ui::BrightnessWindow *ui;
};

#endif // BRIGHTNESSWINDOW_H
