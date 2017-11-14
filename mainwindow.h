#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "messagewindow.h"
#include "imageviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage *img;

    int valuee;
private slots:
    void on_openFile_Clicked();
    void on_saveFile_Clicked();
    void on_saveFileAs_Clicked();

    void on_pushButton_clicked();

private:

    Ui::MainWindow      *ui;
    MessageWindow       *mw;
    ImageViewer         *iv;

protected:
//    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
