#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    debugLabel = new QLabel();
    ui->statusBar->addWidget(debugLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{


//    QApplication::processEvents();
//    ui->label->setText(QString::number(0));



////    QMessageBox::StandardButton reply;
////    reply = QMessageBox::question(this, "Test", "Quit?",
////                                  QMessageBox::Yes|QMessageBox::No);
////    if (reply == QMessageBox::Yes) {
////      qDebug() << "Yes was clicked";
////      QApplication::quit();
////    } else {
////      qDebug() << "Yes was *not* clicked";
////    }

//}

//void MainWindow::on_openImageButton_clicked()
//{


//}

//void MainWindow::on_pushButton_2_clicked()
//{
//    iv = new ImageViewer;
//    iv->show();
//}

//void MainWindow::on_pushButton_clicked()


void MainWindow::on_openFile_Clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), "/", "Image Files (*.png *.jpg *.bmp)");
    img = new QImage(fileName);
//    ui->debugLabel->setScaledContents(true);
    QPixmap p = QPixmap::fromImage(*img);
    ui->verticalScrollBar->setMaximum(img->height());
    ui->horizontalScrollBar->setMaximum(img->width());
    ui->imageLabel->setImage(*img);
    QImage::Format f = img->format();
    if(f == QImage::Format_RGB32) {
        ui->listWidget->addItem("R");
        ui->listWidget->addItem("G");
        ui->listWidget->addItem("B");
        ui->listWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(2)->setCheckState(Qt::CheckState::Checked);
    } else {
        qDebug() << "unknown format" << QString::number(f);
    }

//    ui->imageLabel->setPixmap(p.scaled(ui->imageLabel->width(), ui->imageLabel->height(), Qt::KeepAspectRatio));
//    ui->debugLabel->adjustSize();

//    ui->debugLabel->setText("openFile");
}


//void MainWindow resizeEvent(QResizeEvent *event) {

//}

void MainWindow::on_saveFile_Clicked() {
    debugLabel->setText("saveFile");
}
void MainWindow::on_saveFileAs_Clicked() {
    debugLabel->setText("saveFileAs");
}



void MainWindow::on_RGB_Checked() {
    if(ui->actionYCbCr->isChecked()) {
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form YCbCr to RGB");
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionHSV->setChecked(false);
        debugLabel->setText("form HSV to RGB");
        //actions
    } else {
        ui->actionRGB->setChecked(true);
        debugLabel->setText("RGB isChecked");
        // actions
    }

}

void MainWindow::on_YCbCr_Checked() {
    if(ui->actionRGB->isChecked()) {
        ui->actionRGB->setChecked(false);
        debugLabel->setText("form RGB to YCbCr");
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionHSV->setChecked(false);
        debugLabel->setText("form HSV to YCbCr");
        //actions
    } else {
        ui->actionYCbCr->setChecked(true);
        debugLabel->setText("YCbCr isChecked");
        //actions
    }
}

void MainWindow::on_HSV_Checked() {
    if(ui->actionYCbCr->isChecked()) {
        ui->actionHSV->setChecked(false);
        debugLabel->setText("form YCbCr to RGB");
        //actions
    } else if(ui->actionRGB->isChecked()) {
        ui->actionRGB->setChecked(false);
        debugLabel->setText("form HSV to RGB");
        //actions
    } else {
        ui->actionHSV->setChecked(true);
        debugLabel->setText("HSV isChecked");
        //actions
    }
}

void MainWindow::on_pushButton_clicked()
{
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   ui->imageLabel->resize(ui->imageGroupBox->width(), ui->imageGroupBox->height());
   //    ui->imageLabel->setMaximumWidth(ui->imageGroupBox->width());

   qDebug()<<"MainWindow::resizeEvent";
   // Your code here.
}
