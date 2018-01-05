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
    openFile(fileName);
    event->acceptProposedAction();
}

void MainWindow::openFile(QString fileName) {
    if(img != nullptr)
    {
        delete img;
    }
    img = new QImage();
    if(img->load(fileName)) {
        data = new ImgData(img);
        ui->imageLabel->setImage(img);
//        *loc_img = img->copy();
        if(img->format() == QImage::Format_RGB32) {
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
        } else {
            qDebug() << "unknown format";
        }
    } else {
        delete img;
        ui->imageLabel->setText("Can't handle this format.");
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
    if(img != nullptr)
    {
        bw = new BrightnessWindow(img, 0);
        bw->setAttribute(Qt::WA_DeleteOnClose);
        bw->show();
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

                int num = 0;
                if(item->text() == "R") num = 0;
                if(item->text() == "G") num = 1;
                if(item->text() == "B") num = 2;
                ui->imageLabel->setImage(data->turnOffChannel(num));
                debugLabel->setText(QString::number(num));
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
}

//void MainWindow::on_listWidget_clicked(const QModelIndex &index)
//{
//    QListWidgetItem *item = ui->listWidget->item(index.row());
//}
