#include "brightnesswindow.h"
#include "ui_brightnesswindow.h"

#include <iostream>
//#include <QMessageBox>
#include <QDebug>
#include <QVector>
//void changeImage(QImage *img, int val);

BrightnessWindow::BrightnessWindow(QImage *newImage, int value, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrightnessWindow)
{
    ui->setupUi(this);
    ui->channelBox->addItem("Red");
    ui->channelBox->addItem("Green");
    ui->channelBox->addItem("Blue");
//    img = newImage;
//    *local = img->copy();

//    ui->imageLabel->setImage(local);
}

BrightnessWindow::~BrightnessWindow()
{
    delete ui;
}

void BrightnessWindow::on_applyButton_clicked()
{
    this->close();
}

void BrightnessWindow::on_cancelButton_clicked()
{
    this->close();
}

void BrightnessWindow::on_valueSlider_valueChanged(int value)
{
    this->value = value;
    ui->valueEdit->setText(QString::number(this->value));
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
        } else if(val < 0) {
            value = 0;
            ui->valueSlider->setValue(0);
            ui->valueEdit->setText("0");
        } else {
            value = val;
            ui->valueSlider->setValue(val);
        }
    }
}


void BrightnessWindow::changeImage() {

//        for(int i = 0; i < img->width(); ++i) {
//            for(int j = 0; j< img->height(); ++j){
//                QRgb pix = img->pixel(i, j);
//                int red = qRed(pix);
//                int green = qGreen(pix);
//                int blue = qBlue(pix);

//                local->setPixel(i, j, qRgb(red + value, green + value, blue + value));
//            }
//        }
}


void BrightnessWindow::on_channelBox_currentIndexChanged(int index)
{

}
