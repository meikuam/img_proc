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
    ui->comboBox->addItem("Мера однородности U");
    ui->comboBox->addItem("Энтропия");
    ui->comboBox->addItem("Стандартное отклонение");
    ui->comboBox->addItem("Среднее");
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
    case Uniformity:
    {
        stat_data->setName("Stat Uniformity " + stat_data->getName());
        bin_data->setName("Bin stat Uniformity " + bin_data->getName());
        filtred_data->setName("Filtred stat Uniformity " + filtred_data->getName());
        out_data->setName("Out Uniformity " + out_data->getName());
        //мера однородности
        break;
    }
    case Entropy:
    {
        stat_data->setName("Stat Entropy " + stat_data->getName());
        bin_data->setName("Bin stat Entropy " + bin_data->getName());
        filtred_data->setName("Filtred stat Entropy " + filtred_data->getName());
        out_data->setName("Out Entropy " + out_data->getName());
        //энтропия
        break;
    }
    case StandardDeviation:
    {
        stat_data->setName("Stat StandardDeviation " + stat_data->getName());
        bin_data->setName("Bin stat StandardDeviation " + bin_data->getName());
        filtred_data->setName("Filtred stat StandardDeviation " + filtred_data->getName());
        out_data->setName("Out StandardDeviation " + out_data->getName());
        //стандартное отклонение
        break;
    }
    case Mean:
    {
        stat_data->setName("Stat Mean " + stat_data->getName());
        bin_data->setName("Bin stat Mean " + bin_data->getName());
        filtred_data->setName("Filtred stat Mean " + filtred_data->getName());
        out_data->setName("Out Mean " + out_data->getName());
        //среднее
        break;
    }
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
    case 2:
        method_ = Uniformity;
        break;
    case 3:
        method_ = Entropy;
        break;
    case 4:
        method_ = StandardDeviation;
        break;
    case 5:
        method_ = Mean;
        break;
    }
}
