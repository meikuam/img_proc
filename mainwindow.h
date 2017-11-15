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
}





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel* debugLabel;
    QImage *img;

    int valuee;
private slots:
    void on_openFile_Clicked();
    void on_saveFile_Clicked();
    void on_saveFileAs_Clicked();

    void on_RGB_Checked();
    void on_YCbCr_Checked();
    void on_HSV_Checked();



    void on_pushButton_clicked();

    void resizeEvent(QResizeEvent* event);


private:

    Ui::MainWindow      *ui;
    MessageWindow       *mw;

protected:
};


#endif // MAINWINDOW_H
