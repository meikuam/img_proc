#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "brightnesswindow.h"
#include "filterwindow.h"
#include "imgdata.h"

#include <QtWidgets>
#include <QWheelEvent>
#include <qlabel.h>

namespace Ui {
class MainWindow;
class BrightnessWindow;
class FilterWindow;
class ImageLabel;
class ImgData;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel              *debugLabel;
    vector<ImgData*>    data;

private slots:
    void on_openFile_Clicked();
    void on_saveFile_Clicked();
    void on_saveFileAs_Clicked();

    void on_RGB_Checked();
    void on_YCbCr_Checked();
    void on_HSV_Checked();
    void on_Brightness_Clicked();
    void on_Filters_Clicked();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void setMenuEnabled(bool);
    void on_windowChannels_Clicked();
    void on_windowLayers_Clicked();

    void setUIFormat(Format format);

    void on_layersWidget_itemClicked(QListWidgetItem *item);
    ImgData*  getCurrentLayer();
    int  getCurrentLayerId();
    void addLayer(QString fileName);
    void addLayer(ImgData* img);
    void on_deleteLayerButton_clicked();

    void on_channelsWidget_itemClicked(QListWidgetItem *item);

private:

    Ui::MainWindow      *ui;
    BrightnessWindow    *bw;
    FilterWindow        *fw;

protected:
};


#endif // MAINWINDOW_H
