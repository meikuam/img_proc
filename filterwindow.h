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
    explicit FilterWindow(ImgData *inp, QWidget *parent = 0 );
    ~FilterWindow();
    ImgData *input,
            *output;

    Method  method;
//    int value;
//    bool channels[3];

    Q_SIGNAL
    void setImgData(ImgData* out);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_filterBox_currentIndexChanged(int index);

private:
    Ui::FilterWindow        *ui;
    vector<QTableWidget*>   tables;
    vector<QLabel*>         labels;
};

#endif // FILTERWINDOW_H
