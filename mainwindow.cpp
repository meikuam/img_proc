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
    if (event->mimeData()->hasUrls()) {
        QString fileName  = event->mimeData()->urls()[0].toLocalFile();
        addLayer(fileName);
        event->acceptProposedAction();
    }
}

void MainWindow::on_openFile_Clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение"),
                                                    "/", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    addLayer(fileName);
}

void MainWindow::on_saveFile_Clicked() {
    debugLabel->setText("saveFile");
    ImgData* buf = getCurrentLayer();
    buf->img()->save(buf->getPath());
}

void MainWindow::on_saveFileAs_Clicked() {
    debugLabel->setText("saveFileAs");
    ImgData* buf = getCurrentLayer();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                 buf->getPath(),
                                 tr("Images (*.png *.jpg *.jpeg *.bmp)"));
//    QString path = QFileDialog::getSaveFileUrl(this, tr("Сохранить как.."), "/");//, "Image Files (*.png *.jpg *.jpeg *.bmp)");
    buf->img()->save(fileName);
}

void MainWindow::on_Brightness_Clicked() {

    bw = new BrightnessWindow(getCurrentLayer(), ui->channelsWidget);
    connect(bw, SIGNAL(repaint()),
            ui->imageLabel, SLOT(repaint()));
    connect(bw, SIGNAL(setImage(QImage*)),
            ui->imageLabel, SLOT(setImage(QImage*)));

    bw->setAttribute(Qt::WA_DeleteOnClose);
    bw->show();
    on_windowChannels_Clicked();
}

void MainWindow::on_Filters_Clicked() {
    fw = new FilterWindow(getCurrentLayer());
    connect(fw, SIGNAL(setImgData(ImgData*)),
            this, SLOT(addLayer(ImgData*)));
    fw->setAttribute(Qt::WA_DeleteOnClose);
    fw->show();
    on_windowLayers_Clicked();
}
void MainWindow::on_transforms_Clicked() {
    tw = new TransformsWindow(getCurrentLayer());
    connect(tw, SIGNAL(setImgData(ImgData*)),
            this, SLOT(addLayer(ImgData*)));
    tw->setAttribute(Qt::WA_DeleteOnClose);
    tw->show();
    on_windowLayers_Clicked();
}


void MainWindow::on_search_Clicked() {
    sw = new SearchWindow(getCurrentLayer());
    connect(sw, SIGNAL(setImgData(ImgData*)),
            this, SLOT(addLayer(ImgData*)));
    sw->setAttribute(Qt::WA_DeleteOnClose);
    sw->show();
    on_windowLayers_Clicked();
}

// ----------------------------------------------------
// ------------------ layers --------------------------
void MainWindow::addLayer(QString fileName) {
    data.push_back(new ImgData(fileName));
    if(!data.back()->isNull()) {
        setUIFormat(data.back()->format());
        setMenuEnabled(true);
        ui->layersWidget->addItem(data.back()->getName());

        ui->imageLabel->setImage(data.back()->img());
        for(int i = 0; i < ui->layersWidget->count(); i++) {
            ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Unchecked);
        }
        ui->layersWidget->item(ui->layersWidget->count() - 1)->setCheckState(Qt::CheckState::Checked);
    } else {
        if(ui->layersWidget->count() == 0) {
            ui->imageLabel->setText("Can't handle this format.");
        }
        data.pop_back();
    }
}

void MainWindow::addLayer(ImgData* img) {
    data.push_back(img);
    if(!data.back()->isNull()) {
        setUIFormat(data.back()->format());
        setMenuEnabled(true);
        ui->layersWidget->addItem(data.back()->getName());

        ui->imageLabel->setImage(data.back()->img());
        for(int i = 0; i < ui->layersWidget->count(); i++) {
            ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Unchecked);
        }
        ui->layersWidget->item(ui->layersWidget->count() - 1)->setCheckState(Qt::CheckState::Checked);
    } else {
        data.pop_back();
    }
}

ImgData*  MainWindow::getCurrentLayer() {
    int num = getCurrentLayerId();
    if(num != -1) {
        return data[num];
    } else {
        return nullptr;
    }
}

int  MainWindow::getCurrentLayerId() {
    for(int i = 0; i < ui->layersWidget->count(); i++) {
        if(ui->layersWidget->item(i)->checkState() == Qt::CheckState::Checked) {
            return i;
        }
    }
    return -1;
}

void MainWindow::on_layersWidget_itemClicked(QListWidgetItem *item)
{

    for(int i = 0; i < ui->layersWidget->count(); i++) {
        ui->layersWidget->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }
    item->setCheckState(Qt::CheckState::Checked);
    ui->imageLabel->setImage(getCurrentLayer()->img());
    setUIFormat(getCurrentLayer()->format());
}

void MainWindow::on_deleteLayerButton_clicked() {
    if(ui->layersWidget->count() > 1) {
        int lays = ui->layersWidget->count();
        for(int i = 0; i < lays; i++) {
            if(ui->layersWidget->item(i)->checkState() == Qt::CheckState::Checked) {
                data.erase(data.begin() + i);
                delete ui->layersWidget->item(i);
                int num = i > 1 ? i - 1 : 0;

                ui->layersWidget->item(num)->setCheckState(Qt::CheckState::Checked);
                ui->imageLabel->setImage(data[num]->img());
                break;
            }
        }
    } else if (ui->layersWidget->count() == 1) {
        data.erase(data.begin());
        delete ui->layersWidget->item(0);
        ui->imageLabel->setImage(new QImage()); // TODO: memory leak
        setMenuEnabled(false);
    }
}

// ----------------------------------------------------
// ------------------ channels ------------------------
void MainWindow::on_RGB_Checked() {
    getCurrentLayer()->convertTo(Format_RGB);
    ui->imageLabel->repaint();
    setUIFormat(Format_RGB);
}

void MainWindow::on_YCbCr_Checked() {
    getCurrentLayer()->convertTo(Format_YCbCr);
    ui->imageLabel->repaint();
    setUIFormat(Format_YCbCr);
}

void MainWindow::on_HSV_Checked() {
    getCurrentLayer()->convertTo(Format_HSV);
    ui->imageLabel->repaint();
    setUIFormat(Format_HSV);
}

void MainWindow::setUIFormat(Format format) {
    switch (format) {
    case Format_RGB:
        ui->channelsWidget->clear();

        ui->channelsWidget->addItem("RGB");
        ui->channelsWidget->addItem("R");
        ui->channelsWidget->addItem("G");
        ui->channelsWidget->addItem("B");
        ui->channelsWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(3)->setCheckState(Qt::CheckState::Checked);


        ui->actionRGB->setChecked(true);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(false);
        break;
    case Format_HSV:
        ui->channelsWidget->clear();

        ui->channelsWidget->addItem("HSV");
        ui->channelsWidget->addItem("H");
        ui->channelsWidget->addItem("S");
        ui->channelsWidget->addItem("V");
        ui->channelsWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(3)->setCheckState(Qt::CheckState::Checked);


        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(true);
        ui->actionYCbCr->setChecked(false);
        break;
    case Format_YCbCr:
        ui->channelsWidget->clear();

        ui->channelsWidget->addItem("YCbCr");
        ui->channelsWidget->addItem("Y");
        ui->channelsWidget->addItem("Cb");
        ui->channelsWidget->addItem("Cr");
        ui->channelsWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(1)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(2)->setCheckState(Qt::CheckState::Checked);
        ui->channelsWidget->item(3)->setCheckState(Qt::CheckState::Checked);


        ui->actionRGB->setChecked(false);
        ui->actionHSV->setChecked(false);
        ui->actionYCbCr->setChecked(true);
        break;
    }
}

void MainWindow::on_channelsWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text() == ui->channelsWidget->item(0)->text() &&
       item->checkState() == Qt::CheckState::Unchecked) {                       // first item
        // actions (turn on all of channels)
        for(int i = 0; i < ui->channelsWidget->count(); i++) {
            ui->channelsWidget->item(i)->setCheckState(Qt::CheckState::Checked);
        }
    } else if(item->checkState() == Qt::CheckState::Checked) {                  // current item is checked
        // check if one of channels is on
        bool check = false;
        for(int i = 1; i < ui->channelsWidget->count(); i++) {
            if(ui->channelsWidget->item(i)->checkState() == Qt::CheckState::Checked) {
                if(item->text() != ui->channelsWidget->item(i)->text()) {
                    check = true;
                    break;
                }
            }
        }
        if(check) {
            // actions (turn off one of channels)
            ui->channelsWidget->item(0)->setCheckState(Qt::CheckState::Unchecked);
            item->setCheckState(Qt::CheckState::Unchecked);
        }
    } else {                                                                   // current item is unchecked
        // actions (turn on one of channels)
        item->setCheckState(Qt::CheckState::Checked);
        // check if all of channels are on
        bool check = true;
        for(int i = 1; i < ui->channelsWidget->count(); i++) {
            if(ui->channelsWidget->item(i)->checkState() == Qt::CheckState::Unchecked) {
                check = false;
                break;
            }
        }
        if(check) {
            ui->channelsWidget->item(0)->setCheckState(Qt::CheckState::Checked);
        }
    }


    for(int i = 0; i < ui->layersWidget->count(); i++) {
        if(ui->layersWidget->item(i)->checkState() == Qt::CheckState::Checked) {
            data[i]->setChannels(ui->channelsWidget->item(1)->checkState() == Qt::CheckState::Checked,
                                 ui->channelsWidget->item(2)->checkState() == Qt::CheckState::Checked,
                                 ui->channelsWidget->item(3)->checkState() == Qt::CheckState::Checked);
            ui->imageLabel->setImage(data[i]->img());
        }
    }
}

// ----------------------------------------------------
// -------------------- UI ----------------------------
void MainWindow::on_windowChannels_Clicked() {
    if(ui->action_channels->isChecked()) {
        ui->action_channels->setChecked(false);
        ui->toolBox->setCurrentIndex(1);
    } else {
        ui->action_channels->setChecked(true);
        ui->toolWidget->show();
        ui->toolBox->setCurrentIndex(0);
    }
}

void MainWindow::on_windowLayers_Clicked() {
    if(ui->action_channels->isChecked()) {
        ui->action_channels->setChecked(false);
        ui->toolBox->setCurrentIndex(0);
    } else {
        ui->action_channels->setChecked(true);
        ui->toolWidget->show();
        ui->toolBox->setCurrentIndex(1);
    }
}

void MainWindow::setMenuEnabled(bool sw) {
    //ui->menu_file // Файл
    ui->saveFileAct->setEnabled(sw);
    ui->saveFileAsAct->setEnabled(sw);

    //ui->menu_image // Изображение
    ui->menu_format->setEnabled(sw);
    ui->menu_correction->setEnabled(sw);
    ui->action_transforms->setEnabled(sw);
    ui->action_search->setEnabled(sw);
    //ui->menu_window // Окно
    ui->action_channels->setEnabled(sw);
    ui->action_layers->setEnabled(sw);

    ui->toolWidget->show();
}


//MainWindow::mouseDoubleClickEvent( QMouseEvent * e )
//{
//    if ( e->button() == Qt::LeftButton )
//    {
//        ...
//    }
//}
