#include "brightnesswindow.h"
#include "ui_brightnesswindow.h"

#include <iostream>
//#include <QMessageBox>
#include <QDebug>
#include <QVector>
//void changeImage(QImage *img, int val);

BrightnessWindow::BrightnessWindow(ImgData *data, QListWidget* listWidget, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrightnessWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < listWidget->count(); i++) {
        ui->channelBox->addItem(listWidget->item(i)->text());
    }
    img = data;
    local = new ImgData(*data);
}

BrightnessWindow::~BrightnessWindow()
{
    std::cout<<"~BrightnessWindow()"<<endl;
    delete ui;
}

void BrightnessWindow::on_applyButton_clicked()
{
    delete local;
    this->close();
}

void BrightnessWindow::on_cancelButton_clicked()
{
    delete img;
    img = local;
    setImage(img->toImage());
    repaint();
    this->close();
}

void BrightnessWindow::on_valueSlider_valueChanged(int value)
{
    this->value = value;
    ui->valueEdit->setText(QString::number(this->value));
    std::cout<<"value"<<value<<std::endl;
    changeImage();
}

void BrightnessWindow::on_valueEdit_textChanged(const QString &arg1)
{
    bool ok;
    int val = arg1.toInt(&ok);
    if(ok) {
        if(val > 255) {
            value = 255;
            ui->valueSlider->setValue(255);
            ui->valueEdit->setText("255");
        } else if(val < -255) {
            value = -255;
            ui->valueSlider->setValue(-255);
            ui->valueEdit->setText("-255");
        } else {
            value = val;
            ui->valueSlider->setValue(val);
        }
    }
}


void BrightnessWindow::changeImage() {

        for(int i = 0; i < local->data()->width(); ++i) {
            for(int j = 0; j< local->data()->height(); ++j){
                QRgb p = local->data()->pixel(i, j);
                int red = channels[0] ? qRed(p) + value : qRed(p);
                int green = channels[1] ? qGreen(p) + value : qGreen(p);
                int blue = channels[2] ? qBlue(p) + value : qBlue(p);

                red = red > 255 ? 255 : red < 0 ? 0 : red;
                green = green > 255 ? 255 : green < 0 ? 0 : green;
                blue = blue > 255 ? 255 : blue < 0 ? 0 : blue;
                img->data()->setPixel(i, j, qRgb(red, green, blue));
            }
        }
        setImage(img->toImage());
        repaint();
}


void BrightnessWindow::on_channelBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        channels[0] = channels[1] = channels[2] = true;
        break;
    case 1:
        channels[0] = true;
        channels[1] = channels[2] = false;
        break;
    case 2:
        channels[1] = true;
        channels[0] = channels[2] = false;
        break;
    case 3:
        channels[2] = true;
        channels[0] = channels[1] = false;
        break;
    }

}
