#include "segmentationwindow.h"
#include "ui_segmentationwindow.h"

#include <iostream>


SegmentationWindow::SegmentationWindow(ImgData *inp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SegmentationWindow)
{
    ui->setupUi(this);
    input = inp;
    ui->comboBox->addItem("Третий момент");
    ui->comboBox->addItem("Дескрипторы относительной гладкости R");
}

SegmentationWindow::~SegmentationWindow()
{
    delete ui;
}

void SegmentationWindow::on_applyButton_clicked()
{
    switch (method_) {
    case ThirdMoment:
    {
        break;
    }
    case DesctiptorR:
    {
        break;
    }
    }
}

void SegmentationWindow::on_cancelButton_clicked()
{
    this->close();
}


void SegmentationWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        method_ = ThirdMoment;
        break;
    case 1:
        method_ = DesctiptorR;
        break;
    }
}
