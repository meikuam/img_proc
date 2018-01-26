#include "segmentationwindow.h"
#include "ui_segmentationwindow.h"

#include <iostream>


SegmentationWindow::SegmentationWindow(ImgData *src, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SegmentationWindow)
{
    ui->setupUi(this);
    src_data = src;
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

    stat_data       = new ImgData(*src_data);
    bin_data        = new ImgData(*src_data);
    filtred_data    = new ImgData(*src_data);
    out_data        = new ImgData(*src_data);
    switch (method_) {
    case ThirdMoment:
        stat_data->setName("Stat ThirdMoment " + stat_data->getName());
        bin_data->setName("Bin stat ThirdMoment " + bin_data->getName());
        filtred_data->setName("Filtred stat ThirdMoment " + filtred_data->getName());
        out_data->setName("Out ThirdMoment " + out_data->getName());
        break;
    case DesctiptorR:
        stat_data->setName("Stat DesctiptorR " + stat_data->getName());
        bin_data->setName("Bin stat DesctiptorR " + bin_data->getName());
        filtred_data->setName("Filtred stat DesctiptorR " + filtred_data->getName());
        out_data->setName("Out DesctiptorR " + out_data->getName());
        break;
    }


    Segmentation::segmentation(src_data,
                               stat_data, bin_data, filtred_data, out_data,
                               method_,
                               ui->spinNeighbourBox->value(),
                               ui->spinMinSquareBox->value(),
                               ui->checkUseLocalHistBox->isChecked(),
                               ui->checkMultithreadBox->isChecked());
    setImgData(stat_data);
    setImgData(bin_data);
    setImgData(filtred_data);
    setImgData(out_data);

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
