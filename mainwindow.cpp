#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);
    setMenuEnabled(false);

//    data.push_back(new ImgData());
    debugLabel = new QLabel();
    ui->statusBar->addWidget(debugLabel);

    ui->verticalScrollBar->setHidden(true);
    ui->horizontalScrollBar->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
//    delete data;
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
                                                    "/", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    openFile(fileName);
}

void MainWindow::on_saveFile_Clicked() {
    debugLabel->setText("saveFile");
}

void MainWindow::on_saveFileAs_Clicked() {
    debugLabel->setText("saveFileAs");
}

void MainWindow::openFile(QString fileName) {
    data.push_back(new ImgData(fileName));
//    data.back()->load(fileName);

    if(!data.back()->isNull()) {
        ui->imageLabel->setImage(data.back()->img());
        curr_data = data.size() - 1;
        setlistWidget(Format_RGB);
        setMenuEnabled(true);
        on_RGB_Checked();
        addlayerToWidget(fileName);

//        setlayersWidget();

    } else {
        ui->imageLabel->setText("Can't handle this format.");
        data.pop_back();
    }
}



void MainWindow::on_RGB_Checked() {

    if(ui->actionYCbCr->isChecked()) {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form YCbCr to RGB");

        data[curr_data]->convertTo(Format_RGB);
        setlistWidget(Format_RGB);
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form HSV to RGB");
        data[curr_data]->convertTo(Format_RGB);

        setlistWidget(Format_RGB);
        //actions
    } else {
        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("RGB isChecked");
        data[curr_data]->convertTo(Format_RGB);
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
        data[curr_data]->convertTo(Format_YCbCr);

        setlistWidget(Format_YCbCr);
        //actions
    } else if(ui->actionHSV->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);
        debugLabel->setText("form HSV to YCbCr");
        data[curr_data]->convertTo(Format_YCbCr);

        setlistWidget(Format_YCbCr);
        //actions
    } else {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);
        debugLabel->setText("YCbCr isChecked");
        data[curr_data]->convertTo(Format_YCbCr);
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
        data[curr_data]->convertTo(Format_HSV);
        setlistWidget(Format_HSV);
        //actions
    } else if(ui->actionRGB->isChecked()) {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        debugLabel->setText("form RGB to HSV");
        data[curr_data]->convertTo(Format_HSV);
        setlistWidget(Format_HSV);
        //actions
    } else {
        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        data[curr_data]->convertTo(Format_HSV);
        debugLabel->setText("HSV isChecked");
        //actions
    }
    ui->imageLabel->repaint();
}

void MainWindow::on_Brightness_Clicked() {
    bw = new BrightnessWindow(data[curr_data], ui->listWidget);
    connect(bw, SIGNAL(repaint()),
            ui->imageLabel, SLOT(repaint()));
    connect(bw, SIGNAL(setImage(QImage*)),
            ui->imageLabel, SLOT(setImage(QImage*)));

//    bw->setAttribute(Qt::WA_DeleteOnClose);
    bw->show();

}

void MainWindow::on_Filters_Clicked() {
    data.push_back(new ImgData(*(data[curr_data])));
    addlayerToWidget("Фильтрованое");

    fw = new FilterWindow(data[curr_data], data[curr_data + 1]);
    connect(fw, SIGNAL(repaint()),
            ui->imageLabel, SLOT(repaint()));
    connect(fw, SIGNAL(setImage(QImage*)),
            ui->imageLabel, SLOT(setImage(QImage*)));
    fw->show();
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
    data[curr_data]->setChannels(ui->listWidget->item(1)->checkState() == Qt::CheckState::Checked,
                      ui->listWidget->item(2)->checkState() == Qt::CheckState::Checked,
                      ui->listWidget->item(3)->checkState() == Qt::CheckState::Checked);
    ui->imageLabel->setImage(data[curr_data]->img());
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
    ui->action_layers->setChecked(sw);
    ui->action_channels->setChecked(sw);
}


//MainWindow::mouseDoubleClickEvent( QMouseEvent * e )
//{
//    if ( e->button() == Qt::LeftButton )
//    {
//        ...
//    }
//}

void MainWindow::setlayersWidget() {
    ui->layersWidget->clear();
//    ui->layersWidget->addItem("test");
//    ui->layersWidget->item(0)->setCheckState(Qt::CheckState::Checked);
}

void MainWindow::addlayerToWidget(QString name) {
    ui->layersWidget->addItem(name.section("/",-1,-1));
    int num = ui->layersWidget->count() - 1;
    for(int i = 0; i < ui->layersWidget->count(); i++) {
        ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }
    ui->layersWidget->item(num)->setCheckState(Qt::CheckState::Checked);
}

void MainWindow::on_layersWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::CheckState::Checked) {

//        bool check = false;
//        for(int i = 0; i < ui->layersWidget->count(); i++) {
//            if(ui->listWidget->item(i)->checkState() == Qt::CheckState::Checked) {
//                if(item->text() != ui->layersWidget->item(i)->text()) {
//                    check = true;
//                    break;
//                }
//            }
//        }
//        if(check) {
//            item->setCheckState(Qt::CheckState::Unchecked);
//        }
    } else {
        for(int i = 0; i < ui->layersWidget->count(); i++) {
            ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Unchecked);
        }
        item->setCheckState(Qt::CheckState::Checked);
        for(int i = 0; i < ui->layersWidget->count(); i++) {
            if(ui->layersWidget->item(i)->checkState() == Qt::CheckState::Checked) {
//                std::cout<<i/*<<" "<<ui->layersWidget->item(i)->text()*/<<std::endl;
                ui->imageLabel->setImage(data[i]->img());
                ui->imageLabel->repaint();
                curr_data = i;
                setlistWidget(data[i]->format());
            }
        }
    }

//    ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Checked);
}


void MainWindow::on_windowChannels_Clicked() {
    if(ui->action_channels->isChecked()) {
        ui->action_channels->setChecked(false);
        ui->toolBox->setCurrentIndex(1);
        ui->channels->setVisible(false);
    } else {
        ui->action_channels->setChecked(true);
        ui->toolWidget->show();
        ui->channels->setVisible(true);
        ui->toolBox->setCurrentIndex(0);
    }
}

void MainWindow::on_windowLayers_Clicked() {
    if(ui->action_channels->isChecked()) {
        ui->action_channels->setChecked(false);
        ui->toolBox->setCurrentIndex(0);
        ui->layers->setVisible(false);
    } else {
        ui->action_channels->setChecked(true);
        ui->toolWidget->show();
        ui->layers->setVisible(true);
        ui->toolBox->setCurrentIndex(1);
    }
}

void MainWindow::on_deleteLayerButton_clicked() {
    for(int i = 0; i < ui->layersWidget->count(); i++) {
//        if(ui->layersWidget->item(i)->checkState() == Qt::CheckState::Checked) {
                data.erase(data.begin() + i);
                delete ui->layersWidget->item(i);
//        }
    }
}
