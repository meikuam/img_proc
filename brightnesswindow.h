#ifndef BRIGHTNESSWINDOW_H
#define BRIGHTNESSWINDOW_H

#include <QMainWindow>

namespace Ui {
class BrightnessWindow;
}

class BrightnessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrightnessWindow(QImage *newImage, int value, QWidget *parent = 0 );
    ~BrightnessWindow();
    QImage* img,
            *local;
    int value;

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
