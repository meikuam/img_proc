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
    data = new ImgData();

    debugLabel = new QLabel();
    ui->statusBar->addWidget(debugLabel);
    ui->verticalScrollBar->setHidden(true);
    ui->horizontalScrollBar->setHidden(true);

    ui->menu_format->setEnabled(false);
    ui->menu_correction->setEnabled(false);
    ui->saveFileAct->setEnabled(false);
    ui->saveFileAsAct->setEnabled(false);
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
    openFile(fileName);
    event->acceptProposedAction();
}

void MainWindow::openFile(QString fileName) {
    if(!data->isNull()) {
        qDebug() << "!data->isNull() -> delete data";
        delete data;
    }
    data = new ImgData(fileName);
//    ImgData* copy = new ImgData(*data);

    if(!data->isNull()) {
        ui->imageLabel->setImage(data->toImage());
//        *loc_img = img->copy();
        if(data->format() == Format_RGB) {
            setlistWidget(data->format());

            ui->menu_format->setEnabled(true);
            ui->menu_correction->setEnabled(true);
            ui->saveFileAct->setEnabled(true);
            ui->saveFileAsAct->setEnabled(true);
        } else {
            delete data;
            qDebug() << "unknown format";
        }
    } else {
        delete data;
        ui->imageLabel->setText("Can't handle this format.");
    }
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

        ui->listWidget->selectAll();
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

        ui->listWidget->selectAll();
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

        ui->listWidget->selectAll();
        break;
    default:
        break;
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


void MainWindow::on_Brightness_Clicked() {
    if(data != nullptr)
    {
        bw = new BrightnessWindow(data, ui->listWidget);
        bw->setAttribute(Qt::WA_DeleteOnClose);
        bw->show();
    }
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
    ui->imageLabel->setImage(data->toImage(ui->listWidget->item(1)->checkState() == Qt::CheckState::Checked,
                                           ui->listWidget->item(2)->checkState() == Qt::CheckState::Checked,
                                           ui->listWidget->item(3)->checkState() == Qt::CheckState::Checked));
}

//void MainWindow::on_listWidget_clicked(const QModelIndex &index)
//{
//    QListWidgetItem *item = ui->listWidget->item(index.row());
//}
