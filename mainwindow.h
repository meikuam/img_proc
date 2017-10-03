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
    void on_pushButton_clicked();

    void on_openImageButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow      *ui;
    MessageWindow       *mw;
    ImageViewer         *iv;
};

#endif // MAINWINDOW_H
