#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "imgdata.h"
#include "search.h"
#include "filter.h"


namespace Ui {
class SearchWindow;
}

class SearchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchWindow(ImgData *inp, QWidget *parent = 0 );
    ~SearchWindow();
    ImgData *input,
            *output;

    Q_SIGNAL
    void setImgData(ImgData* out);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

    void on_getMaskButton_clicked();

    private:
    Ui::SearchWindow    *ui;
};

#endif // SEARCHWINDOW_H
