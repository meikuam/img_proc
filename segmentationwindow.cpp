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

    qRegisterMetaType<ImgData*>("ImgData*");
    QSignalSpy spy(this, SIGNAL(setImgData(ImgData*)));


    neighbourSize   = ui->spinNeighbourBox->value();
    minSquare       = ui->spinMinSquareBox->value();

    output = new ImgData(*input);
    Filter::filter(input, input, Gray);
    Segmentation::segmentation(input, output, method_, neighbourSize);
    setImgData(output);

    bool check = spy.wait(1000);
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
