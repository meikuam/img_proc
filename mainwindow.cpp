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

    debugLabel = new QLabel();
    ui->statusBar->addWidget(debugLabel);
    ui->verticalScrollBar->setHidden(true);
    ui->horizontalScrollBar->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        ui->imageLabel->setText("Drop image here");
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString fileName  = event->mimeData()->urls()[0].toLocalFile();
    QImage test;
    if(test.load(fileName)) {
        openFile(fileName);
    } else {
        ui->imageLabel->setText("Can't handle this format.");
    }
    event->acceptProposedAction();
}

void MainWindow::openFile(QString fileName) {
    img = new QImage(fileName);
    ui->imageLabel->setImage(img);

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
}

void MainWindow::on_openFile_Clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"), "/", "Image Files (*.png *.jpg *.bmp)");
    openFile(fileName);
}

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
    if(img != nullptr)
    {
        mw = new MessageWindow(img, 0);
        mw->setAttribute(Qt::WA_DeleteOnClose);
        mw->show();
    }
}



//MainWindow::mouseDoubleClickEvent( QMouseEvent * e )
//{
//    if ( e->button() == Qt::LeftButton )
//    {
//        ...
//    }
//}


//void MainWindow::resizeEvent(QResizeEvent* event)
//{
//   QMainWindow::resizeEvent(event);

//   // Your code here.
//}
