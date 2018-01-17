#ifndef TRANSFORMSWINDOW_H
#define TRANSFORMSWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "imgdata.h"
#include "transforms.h"
#include "filter.h"


namespace Ui {
class TransformsWindow;
}

class TransformsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransformsWindow(ImgData *inp, QWidget *parent = 0 );
    ~TransformsWindow();
    ImgData *input,
            *output;

    Method  method;

    Q_SIGNAL
    void setImgData(ImgData* out);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
    void on_transformBox_currentIndexChanged(int index);

private:
    Ui::TransformsWindow    *ui;
};

#endif // TRANSFORMSWINDOW_H
