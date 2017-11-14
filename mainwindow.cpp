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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    MainWindow::resizeEvent(event);
//    if(this)
//    {
//        ui->horizontalLayout->SizeConstraint =
//        // QLabel process
//    }
//}


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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), "/", tr("Image Files (*.png *.jpg *.bmp"));
    img = new QImage(fileName);
    ui->debugLabel->setScaledContents(true);
    ui->debugLabel->setPixmap(QPixmap::fromImage(*img));
    ui->debugLabel->adjustSize();

//    ui->debugLabel->setText("openFile");
}
void MainWindow::on_saveFile_Clicked() {
    ui->debugLabel->setText("saveFile");
}
void MainWindow::on_saveFileAs_Clicked() {
    ui->debugLabel->setText("saveFileAs");
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
