#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QMessageBox>
#include <QDebug>
#include "messagewindow.h"
#include "ui_messagewindow.h"

#include <QImage>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);
    valuee = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{


    QApplication::processEvents();
    ui->label->setText(QString::number(0));



//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this, "Test", "Quit?",
//                                  QMessageBox::Yes|QMessageBox::No);
//    if (reply == QMessageBox::Yes) {
//      qDebug() << "Yes was clicked";
//      QApplication::quit();
//    } else {
//      qDebug() << "Yes was *not* clicked";
//    }

}

void MainWindow::on_openImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), "/", tr("Image Files (*.png *.jpg *.bmp"));
    img = new QImage(fileName);
    if(img->isNull())
    {
        return;
    }
    else
    {
        mw = new MessageWindow(*img, 0);
        mw->setAttribute(Qt::WA_DeleteOnClose);
        mw->show();
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    iv = new ImageViewer;
    iv->show();
}
