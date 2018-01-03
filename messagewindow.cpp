#include "messagewindow.h"
#include "ui_messagewindow.h"
#include <QMessageBox>
#include <QDebug>
void changeImage(QImage *img, int val);

MessageWindow::MessageWindow(QImage *newImage, int value, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);

    ui->valueSlider->setMaximum(255);
    ui->valueSlider->setMinimum(0);
    ui->valueSlider->setValue(value);

    img = newImage;
//    *local = img->copy();

    ui->imageLabel->setImage(local);
}

MessageWindow::~MessageWindow()
{
    delete ui;
}



void MessageWindow::on_applyButton_clicked()
{

}

void MessageWindow::on_cancelButton_clicked()
{

}

void MessageWindow::on_valueSlider_valueChanged(int value)
{
    this->value = value;
    ui->valueEdit->setText(QString::number(this->value));
    changeImage();
}

void MessageWindow::on_valueEdit_textChanged(const QString &arg1)
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
    changeImage();
    ui->imageLabel->repaint();
}


void MessageWindow::changeImage() {

        for(int i = 0; i < img->width(); ++i) {
            for(int j = 0; j< img->height(); ++j){
                QRgb pix = img->pixel(i, j);
                int red = qRed(pix);
                int green = qGreen(pix);
                int blue = qBlue(pix);

                local->setPixel(i, j, qRgb(red + value, green + value, blue + value));
            }
        }
}
