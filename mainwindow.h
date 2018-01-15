#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
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

    ImgData             *data;
private slots:
    Q_SLOT
    void on_openFile_Clicked();
    Q_SLOT
    void on_saveFile_Clicked();
    Q_SLOT
    void on_saveFileAs_Clicked();
    Q_SLOT
    void on_RGB_Checked();
    Q_SLOT
    void on_YCbCr_Checked();
    Q_SLOT
    void on_HSV_Checked();
    Q_SLOT
    void on_Brightness_Clicked();

    Q_SLOT
    void on_Filters_Clicked();

    void openFile(QString fileName);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void setlistWidget(Format format);
    void setlayersWidget();
    void setMenuEnabled(bool);

    void on_layersWidget_itemClicked(QListWidgetItem *item);

    void on_windowChannels_Clicked();
    void on_windowLayers_Clicked();

private:

    Ui::MainWindow      *ui;
    MessageWindow       *mw;
    BrightnessWindow    *bw;
    FilterWindow        *fw;

protected:
};


#endif // MAINWINDOW_H
