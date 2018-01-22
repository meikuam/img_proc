#ifndef MORPHWINDOW_H
#define MORPHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QSignalSpy>
#include <QTest>

#include "imgdata.h"
#include "search.h"
#include "filter.h"
#include "morph.h"


namespace Ui {
class MorphWindow;
}

class MorphWindow : public QMainWindow
{
    Q_OBJECT

    Method method_;
    bool ready = true;
public:
    explicit MorphWindow(ImgData *inp, QWidget *parent = 0 );
    ~MorphWindow();
    ImgData *input,
            *mask,
            *output;

    Q_SIGNAL
    void setImgData(ImgData* out);

    Q_SLOT
    void gotReady();

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

    void on_getMaskButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MorphWindow    *ui;
};

#endif // MORPHWINDOW_H
