#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "brightnesswindow.h"
#include "filterwindow.h"
#include "transformswindow.h"
#include "morphwindow.h"
#include "segmentationwindow.h"
#include "imgdata.h"

#include <QtWidgets>
#include <QWheelEvent>
#include <qlabel.h>

namespace Ui {
class MainWindow;
class BrightnessWindow;
class FilterWindow;
class TransformsWindow;
class MorphWindow;
class SegmentationWindow;
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

    Q_SIGNAL
    void ready();

private slots:
    void on_openFileAct_triggered();
    void on_saveFileAct_triggered();
    void on_saveFileAsAct_triggered();

    void on_actionRGB_triggered();
    void on_actionYCbCr_triggered();
    void on_actionHSV_triggered();
    void on_action_Brightness_triggered();
    void on_action_Filters_triggered();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void setMenuEnabled(bool);
    void on_action_channels_triggered();
    void on_action_layers_triggered();

    void setUIFormat(Format format);

    void on_layersWidget_itemClicked(QListWidgetItem *item);
    ImgData*  getCurrentLayer();
    int  getCurrentLayerId();
    void addLayer(QString fileName);
    void addLayer(ImgData* img);
    void on_deleteLayerButton_clicked();

    void on_channelsWidget_itemClicked(QListWidgetItem *item);

    void on_action_transforms_triggered();
    void on_action_morph_triggered();
    void on_action_segmentation_triggered();


private:

    Ui::MainWindow      *ui;
    BrightnessWindow    *bw;
    FilterWindow        *fw;
    TransformsWindow    *tw;
    MorphWindow         *mw;
    SegmentationWindow  *sw;

protected:
};


#endif // MAINWINDOW_H
