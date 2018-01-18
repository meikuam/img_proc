#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <iostream>


SearchWindow::SearchWindow(ImgData *inp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    input = inp;
    ui->comboBox->addItem("успех-неудача");
    ui->comboBox->addItem("Бинаризация");
    ui->comboBox->addItem("Эрозия");
    ui->comboBox->addItem("Дилатация");
    ui->comboBox->addItem("Вычитание");
    ui->comboBox->addItem("Инверсия");
    ui->comboBox->addItem("Объединение");
    ui->comboBox->addItem("Пересечение");
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::on_applyButton_clicked()
{
//    ImgData* output = new ImgData(*input);
    switch (method_) {
    case HitOrMiss:
    {
//        A*B = (A erosion B) intersection (Ac erosion (W substract B))

        ImgData* A = new ImgData(*input);
        Filter::filter(input, A, Binary);               // bin A
        setImgData(A);

        on_getMaskButton_clicked();
        ImgData* AeB = new ImgData(*A);
        ImgData* B = new ImgData(*mask);

        Filter::morphFilter(A, AeB, B, Erosion);     // (A erosion B)
        setImgData(AeB);

        ImgData* Ac = new ImgData(*A);
        Filter::filter(A, Ac, Inversion);               // Ac
        setImgData(Ac);

        ImgData* WB = new ImgData(*B);
        Filter::filter(B, WB, Inversion);            // (W substract B)
        setImgData(WB);

        ImgData* AcWB = new ImgData(*Ac);
        Filter::morphFilter(Ac, AcWB, WB, Erosion);    // (Ac erosion (W substract B))
        setImgData(AcWB);

        ImgData* res = new ImgData(*A);
        Filter::morphFilter(AeB, res, AcWB, Intersection);     // A*B = (A erosion B) intersection (Ac erosion (W substract B))
        setImgData(res);
        break;
    }
    case Inversion:
    case Binary:
    {
        ImgData* bin = new ImgData(*input);
        Filter::filter(input, bin, method_);
        setImgData(bin);
        input = bin;
        break;
    }
    case Erosion:
    case Subtraction:
    case Dilation:
    case Union:
    case Intersection:
    {
        ImgData* bin = new ImgData(*input);
        Filter::filter(input, bin, Binary);
        setImgData(bin);
        ImgData* res = new ImgData(*bin);
        on_getMaskButton_clicked();
        Filter::morphFilter(input, res, mask, method_);
        setImgData(res);
        input = res;
        break;
    }
    }
}

void SearchWindow::on_cancelButton_clicked()
{
    this->close();
}

void SearchWindow::on_getMaskButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение маску"),
                                                    "/", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    mask = new ImgData(fileName);
    setImgData(mask);

    ImgData* bin = new ImgData(*mask);
    Filter::filter(mask, bin, Binary);
    setImgData(bin);
    mask = bin;
}

void SearchWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        method_ = HitOrMiss;
        break;
    case 1:
        method_ = Binary;
        break;
    case 2:
        method_ = Erosion;
        break;
    case 3:
        method_ = Dilation;
        break;
    case 4:
        method_ = Subtraction;
        break;
    case 5:
        method_ = Inversion;
        break;
    case 6:
        method_ = Union;
        break;
    case 7:
        method_ = Intersection;
        break;
    }
}
