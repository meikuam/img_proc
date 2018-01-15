#include "filterwindow.h"
#include "ui_filterwindow.h"

#include <iostream>
#include <QDebug>
#include <QVector>


void FilterWindow::addTableAt(Filter* f, int row, int col) {
    tables.push_back(new QTableWidget(this));
    tables.back()->setRowCount(3);
    tables.back()->setColumnCount(3);
    tables.back()->horizontalHeader()->setDefaultSectionSize(30);

    labels.push_back(new QLabel(this));
    labels.back()->setText("Test");

    ui->gridLayout->addWidget(labels.back(), row, col);
    ui->gridLayout->addWidget(tables.back(), row + 1, col);
}

FilterWindow::FilterWindow(ImgData *data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);
    addTableAt(nullptr, 0, 0);
    addTableAt(nullptr, 0, 1);

    img = data;
    local = new ImgData(*data);
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_applyButton_clicked()
{
    delete local;
    this->close();
}

void FilterWindow::on_cancelButton_clicked()
{
    *img = *local;
    setImage(img->img());
    repaint();
    this->close();
}

//void BrightnessWindow::on_valueSlider_valueChanged(int value)
//{
//    this->value = value;
//    ui->valueEdit->setText(QString::number(this->value));
//    std::cout<<"value"<<value<<std::endl;
//    changeImage();
//}

//void BrightnessWindow::on_valueEdit_textChanged(const QString &arg1)
//{
//    bool ok;
//    int val = arg1.toInt(&ok);
//    if(ok) {
//        if(val > 255) {
//            value = 255;
//            ui->valueSlider->setValue(255);
//            ui->valueEdit->setText("255");
//        } else if(val < -255) {
//            value = -255;
//            ui->valueSlider->setValue(-255);
//            ui->valueEdit->setText("-255");
//        } else {
//            value = val;
//            ui->valueSlider->setValue(val);
//        }
//    }
//}


//void BrightnessWindow::changeImage() {

//        for(int x = 0; x < local->width(); x++) {
//            for(int y = 0; y < local->height(); y++){
//                int a = channels[0] ? (*(*local)(x, y, 0)) + value : (*(*local)(x, y, 0));
//                int b = channels[1] ? (*(*local)(x, y, 1)) + value : (*(*local)(x, y, 1));
//                int c = channels[2] ? (*(*local)(x, y, 2)) + value : (*(*local)(x, y, 2));

//                (*(*img)(x, y, 0))  = a > 255 ? 255 : a < 0 ? 0 : a;
//                (*(*img)(x, y, 1))  = b > 255 ? 255 : b < 0 ? 0 : b;
//                (*(*img)(x, y, 2))  = c > 255 ? 255 : c < 0 ? 0 : c;
//            }
//        }
//        img->RGBupdate();
//        repaint();
//}


//void BrightnessWindow::on_channelBox_currentIndexChanged(int index)
//{
//    switch (index) {
//    case 0:
//        channels[0] = channels[1] = channels[2] = true;
//        break;
//    case 1:
//        channels[0] = true;
//        channels[1] = channels[2] = false;
//        break;
//    case 2:
//        channels[1] = true;
//        channels[0] = channels[2] = false;
//        break;
//    case 3:
//        channels[2] = true;
//        channels[0] = channels[1] = false;
//        break;
//    }
//    value = 0;
//    ui->valueSlider->setValue(0);
//    ui->valueEdit->setText("0");

//}
