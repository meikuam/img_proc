#ifndef SEGMENTATIONWINDOW_H
#define SEGMENTATIONWINDOW_H

#include <QMainWindow>
#include <QLabel>
//#include <QFileDialog>
#include <QSignalSpy>
#include <QTest>

#include "imgdata.h"
#include "filter.h"
#include "segmentation.h"


namespace Ui {
class SegmentationWindow;
}

class SegmentationWindow : public QMainWindow
{
    Q_OBJECT

    Statistic   method_;

public:
    explicit SegmentationWindow(ImgData *src, QWidget *parent = 0 );
    ~SegmentationWindow();
    ImgData *src_data,
            *out_data,
            *stat_data,
            *bin_data,
            *filtred_data;

    Q_SIGNAL
    void setImgData(ImgData* out);


private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SegmentationWindow    *ui;
};

#endif // SEGMENTATIONWINDOW_H
