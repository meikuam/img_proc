#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <iostream>


SearchWindow::SearchWindow(ImgData *inp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchsWindow)
{
    ui->setupUi(this);

    input = inp;
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::on_applyButton_clicked()
{
    ImgData* output = new ImgData(*input);
    Search::search(input, output);
    setImgData(output);
    this->close();
}

void SearchWindow::on_cancelButton_clicked()
{
    this->close();
}

void SearchWindow::on_getMaskButton_clicked()
{

}
