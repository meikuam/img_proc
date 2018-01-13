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
    ui->setupUi(this);

    setAcceptDrops(true);
    setMenuEnabled(false);

    data = new ImgData();
    debugLabel = new QLabel();
    ui->statusBar->addWidget(debugLabel);

    ui->verticalScrollBar->setHidden(true);
    ui->horizontalScrollBar->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete data;
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
    if (event->mimeData()->hasUrls()) {
        QString fileName  = event->mimeData()->urls()[0].toLocalFile();
        openFile(fileName);
        event->acceptProposedAction();
    }
}

void MainWindow::on_openFile_Clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"),
                                                    "/", "Image Files (*.png *.jpg *.bmp)");
    openFile(fileName);
}

void MainWindow::on_saveFile_Clicked() {
    debugLabel->setText("saveFile");
}

void MainWindow::on_saveFileAs_Clicked() {
    debugLabel->setText("saveFileAs");
}

void MainWindow::openFile(QString fileName) {
    data->load(fileName);

    if(!data->isNull()) {
        ui->imageLabel->setImage(data->img());
        setlistWidget(Format_RGB);
        setMenuEnabled(true);
    } else {
        ui->imageLabel->setText("Can't handle this format.");
    }
}



void MainWindow::on_RGB_Checked() {

    if(ui->actionYCbCr->isChecked()) {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form YCbCr to RGB");

        data->convertTo(Format_RGB);
        setlistWidget(Format_RGB);
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form HSV to RGB");
        data->convertTo(Format_RGB);

        setlistWidget(Format_RGB);
        //actions
    } else {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("RGB isChecked");
        data->convertTo(Format_RGB);
        // actions
    }
    ui->imageLabel->repaint();
}

void MainWindow::on_YCbCr_Checked() {

    if(ui->actionRGB->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);

        debugLabel->setText("form RGB to YCbCr");
        data->convertTo(Format_YCbCr);

        setlistWidget(Format_YCbCr);
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);
        debugLabel->setText("form HSV to YCbCr");
        data->convertTo(Format_YCbCr);

        setlistWidget(Format_YCbCr);
        //actions
    } else {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);
        debugLabel->setText("YCbCr isChecked");
        data->convertTo(Format_YCbCr);
        //actions
    }
    ui->imageLabel->repaint();
}

void MainWindow::on_HSV_Checked() {
    if(ui->actionYCbCr->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form YCbCr to HSV");
        data->convertTo(Format_HSV);
        setlistWidget(Format_HSV);
        //actions
    } else if(ui->actionRGB->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form RGB to HSV");
        data->convertTo(Format_HSV);
        setlistWidget(Format_HSV);
        //actions
    } else {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        data->convertTo(Format_HSV);
        debugLabel->setText("HSV isChecked");
        //actions
    }
    ui->imageLabel->repaint();
}

void MainWindow::on_Brightness_Clicked() {
    bw = new BrightnessWindow(data, ui->listWidget);
    connect(bw, SIGNAL(repaint()),
            ui->imageLabel, SLOT(repaint()));
    connect(bw, SIGNAL(setImage(QImage*)),
            ui->imageLabel, SLOT(setImage(QImage*)));

//    bw->setAttribute(Qt::WA_DeleteOnClose);
    bw->show();

}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item) {

    if(item->checkState() == Qt::CheckState::Checked) {                     // current item is checked
        if(item->text() == ui->listWidget->item(0)->text()) {               // first item
            // ignore click //
        } else {                                                            // one of items

            // actions (turn off one of channels)
            ui->listWidget->item(0)->setCheckState(Qt::CheckState::Unchecked);
            // check if one of channels is on
            bool check = false;
            for(int i = 1; i < ui->listWidget->count(); i++) {
                if(ui->listWidget->item(i)->checkState() == Qt::CheckState::Checked) {
                    if(item->text() != ui->listWidget->item(i)->text()) {
                        check = true;
                        break;
                    }
                }
            }
            if(check) {
                item->setCheckState(Qt::CheckState::Unchecked);
            }
        }
    } else {                                                                // current item is unchecked
        if(item->text() == ui->listWidget->item(0)->text()) {               // first item
            // actions (turn on all of channels)
            for(int i = 0; i < ui->listWidget->count(); i++) {
                ui->listWidget->item(i)->setCheckState(Qt::CheckState::Checked);
            }
        } else {                                                            // one of items
            // actions (turn on one of channels)
            item->setCheckState(Qt::CheckState::Checked);
            // check if all of channels are on
            bool check = true;
            for(int i = 1; i < ui->listWidget->count(); i++) {
                if(ui->listWidget->item(i)->checkState() == Qt::CheckState::Unchecked) {
                    check = false;
                    break;
                }
            }
            if(check) {
                ui->listWidget->item(0)->setCheckState(Qt::CheckState::Checked);
            }
        }
    }
    data->setChannels(ui->listWidget->item(1)->checkState() == Qt::CheckState::Checked,
                      ui->listWidget->item(2)->checkState() == Qt::CheckState::Checked,
                      ui->listWidget->item(3)->checkState() == Qt::CheckState::Checked);
    ui->imageLabel->setImage(data->img());
}

void MainWindow::setlistWidget(Format format) {
    switch (format) {
    case Format_RGB:
        ui->listWidget->clear();

        ui->listWidget->addItem("RGB");
        ui->listWidget->addItem("R");
        ui->listWidget->addItem("G");
        ui->listWidget->addItem("B");
        ui->listWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(3)->setCheckState(Qt::CheckState::Checked);
        break;
    case Format_HSV:
        ui->listWidget->clear();

        ui->listWidget->addItem("HSV");
        ui->listWidget->addItem("H");
        ui->listWidget->addItem("S");
        ui->listWidget->addItem("V");
        ui->listWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(3)->setCheckState(Qt::CheckState::Checked);
        break;
    case Format_YCbCr:
        ui->listWidget->clear();

        ui->listWidget->addItem("YCbCr");
        ui->listWidget->addItem("Y");
        ui->listWidget->addItem("Cb");
        ui->listWidget->addItem("Cr");
        ui->listWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->listWidget->item(3)->setCheckState(Qt::CheckState::Checked);
        break;
    }
}


void MainWindow::setMenuEnabled(bool sw) {
    ui->menu_format->setEnabled(sw);
    ui->menu_correction->setEnabled(sw);
    ui->saveFileAct->setEnabled(sw);
    ui->saveFileAsAct->setEnabled(sw);
    ui->toolWidget->show();
}

//void MainWindow::on_pushButton_clicked()
//{
//    if(img != nullptr)
//    {
//        mw = new MessageWindow(img, 0);
//        mw->setAttribute(Qt::WA_DeleteOnClose);
//        mw->show();
//    }
//}



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
