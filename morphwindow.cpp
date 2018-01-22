#include "morphwindow.h"
#include "ui_morphwindow.h"

#include <iostream>


MorphWindow::MorphWindow(ImgData *inp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MorphWindow)
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

MorphWindow::~MorphWindow()
{
    delete ui;
}

void MorphWindow::on_applyButton_clicked()
{
    qRegisterMetaType<ImgData*>("ImgData*");
    QSignalSpy spy(this, SIGNAL(setImgData(ImgData*)));

    switch (method_) {
    case HitOrMiss:
    {

//        A*B = (A erosion B) intersection (Ac erosion (W substract B))

        ImgData* A = new ImgData(*input);
        Filter::filter(input, A, Binary);               // bin A
        setImgData(A);
        bool check = spy.wait(10);

        on_getMaskButton_clicked();
        ImgData* AeB = new ImgData(*A);
        ImgData* B = new ImgData(*mask);
        Morph::morph(A, AeB, B, Erosion);               // (A erosion B)
        setImgData(AeB);
        check = spy.wait(10);

        ImgData* Ac = new ImgData(*A);
        Filter::filter(A, Ac, Inversion);               // Ac
        setImgData(Ac);
        check = spy.wait(10);

        ImgData* WB = new ImgData(*B);
        Filter::filter(B, WB, Inversion);               // (W substract B)
        setImgData(WB);
        check =spy.wait(10);

        ImgData* AcWB = new ImgData(*Ac);
        Morph::morph(Ac, AcWB, WB, Erosion);            // (Ac erosion (W substract B))
        setImgData(AcWB);
        check =spy.wait(10);

        ImgData* res = new ImgData(*A);
        Morph::morph(AeB, res, AcWB, Intersection);     // A*B = (A erosion B) intersection (Ac erosion (W substract B))
        setImgData(res);
        check =spy.wait(10);

        break;
    }
    case Inversion:
    case Binary:
    {
        ImgData* bin = new ImgData(*input);
        Filter::filter(input, bin, method_);
        setImgData(bin);
        bool check = spy.wait(10);
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
        bool check = spy.wait(10);
        ImgData* res = new ImgData(*bin);
        on_getMaskButton_clicked();
        Morph::morph(input, res, mask, method_);
        setImgData(res);

        check = spy.wait(10);
        input = res;
        break;
    }
    }
}

void MorphWindow::on_cancelButton_clicked()
{
    this->close();
}

void MorphWindow::on_getMaskButton_clicked()
{
    qRegisterMetaType<ImgData*>("ImgData*");
    QSignalSpy spy(this, SIGNAL(setImgData(ImgData*)));

    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение маску"),
                                                    "/", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    mask = new ImgData(fileName);
    setImgData(mask);

    bool check = spy.wait(1);
    ImgData* bin = new ImgData(*mask);
    Filter::filter(mask, bin, Binary);
    setImgData(bin);

    check = spy.wait(10);
    mask = bin;
}

void MorphWindow::on_comboBox_currentIndexChanged(int index)
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


void MorphWindow::gotReady() {
    ready = true;
}
