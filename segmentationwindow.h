#ifndef SEGMENTATIONWINDOW_H
#define SEGMENTATIONWINDOW_H

#include <QMainWindow>
#include <QLabel>
//#include <QFileDialog>
#include <QSignalSpy>
#include <QTest>

#include "imgdata.h"
#include "segmentation.h"


namespace Ui {
class SegmentationWindow;
}

class SegmentationWindow : public QMainWindow
{
    Q_OBJECT

    Statistic method_;
    bool ready = true;
public:
    explicit SegmentationWindow(ImgData *inp, QWidget *parent = 0 );
    ~SegmentationWindow();
    ImgData *input,
            *mask,
            *output;

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
