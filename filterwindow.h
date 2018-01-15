#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>

#include "imgdata.h"
#include "filter.h"


namespace Ui {
class FilterWindow;
}

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterWindow(ImgData *inp, ImgData *out, QWidget *parent = 0 );
    ~FilterWindow();
    ImgData *input,
            *output;

    Method  method;
//    int value;
//    bool channels[3];

    Q_SIGNAL
    void repaint();
    Q_SIGNAL
    void setImage(QImage* img);
private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
//    void on_valueSlider_valueChanged(int value);
//    void on_valueEdit_textChanged(const QString &arg1);
//    void changeImage();
//    void on_channelBox_currentIndexChanged(int index);

    void on_filterBox_currentIndexChanged(int index);

private:
    Ui::FilterWindow *ui;
    vector<QTableWidget*>   tables;
    vector<QLabel*>         labels;
    void addTable(Method f);
};

#endif // FILTERWINDOW_H
