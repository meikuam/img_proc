#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include <QtWidgets>
#include <QWheelEvent>
#include <qlabel.h>

namespace Ui {
class MainWindow;
class ImageLabel;
class ImgData;
}


enum Format {
    Format_RGB,
    Format_YCbCr,
    Format_HSV
};


class ImgData{
public:
    Format format = Format_RGB;
    QImage *img;
    bool channels[3];
    ImgData(QImage *ptr){
        img = ptr;
        channels[0] = channels[1] = channels[2] = true;
    }
    QImage* turnOffChannel(int ch) {
        QImage* buf = new QImage(img->copy());
        if(channels[ch]) {
            channels[ch] = false;
            for(int x = 0; x < buf->width(); x++) {
                for(int y = 0; y < buf->height(); y++) {
                    QRgb pix = buf->pixel(x, y);
                    int red = channels[0] ? qRed(pix) : 0;
                    int green = channels[1] ?  qGreen(pix) : 0;
                    int blue = channels[2] ? qBlue(pix) : 0;
                    buf->setPixel(x, y, qRgb(red, green, blue));
                }
            }
        }
        return buf;
    }

};


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

    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

//    void on_listWidget_clicked(const QModelIndex &index);

private:

    Ui::MainWindow      *ui;
    MessageWindow       *mw;

protected:
};


#endif // MAINWINDOW_H
