#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "brightnesswindow.h"
#include "imgdata.h"

#include <QtWidgets>
#include <QWheelEvent>
#include <qlabel.h>

namespace Ui {
class MainWindow;
class BrightnessWindow;
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
    QImage              *img = nullptr;
    QImage              *loc_img = nullptr;

    ImgData             *data;
private slots:
    void on_openFile_Clicked();
    void on_saveFile_Clicked();
    void on_saveFileAs_Clicked();

    void on_RGB_Checked();
    void on_YCbCr_Checked();
    void on_HSV_Checked();

    void on_Brightness_Clicked();

    void openFile(QString fileName);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void on_listWidget_itemClicked(QListWidgetItem *item);

//    void on_listWidget_clicked(const QModelIndex &index);

private:

    Ui::MainWindow      *ui;
    MessageWindow       *mw;
    BrightnessWindow    *bw;

protected:
};


#endif // MAINWINDOW_H
