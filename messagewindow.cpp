#include "messagewindow.h"
#include "ui_messagewindow.h"
#include <QMessageBox>
#include <QDebug>


MessageWindow::MessageWindow(QImage &newImage, int value, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);
    ui->verticalSlider->setValue(value);
    img = newImage;
    ui->label->setBackgroundRole(QPalette::Base);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->horizontalLayout->SetMinimumSize = 1;
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(img));
//    ui->label->adjustSize();
}

MessageWindow::~MessageWindow()
{
    delete ui;
}



void MessageWindow::on_verticalSlider_valueChanged(int value)
{
    this->value = value;
}
