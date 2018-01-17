#include "transformswindow.h"
#include "ui_transformswindow.h"

#include <iostream>


TransformsWindow::TransformsWindow(ImgData *inp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransformsWindow)
{
    ui->setupUi(this);

    input = inp;

    ui->transformBox->addItem("Преобразование Хаара");     // 0
    ui->transformBox->addItem("Преобразование Добеши");    // 1
}

TransformsWindow::~TransformsWindow()
{
    delete ui;
}

void TransformsWindow::on_applyButton_clicked()
{
    ImgData* output = new ImgData(*input);
    setImgData(output);

    this->close();
}

void TransformsWindow::on_cancelButton_clicked()
{
    this->close();
}

void TransformsWindow::on_transformBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
    {
        //Haar
        break;
    }
    case 1:
    {
        //Dobeshi
        break;
    }
    }
}

