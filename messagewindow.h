#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QMainWindow>

namespace Ui {
class MessageWindow;
}

class MessageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessageWindow(QImage &newImage, int value, QWidget *parent = 0 );
    ~MessageWindow();
    QImage img;
    int value;

private slots:
    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MessageWindow *ui;
};

#endif // MESSAGEWINDOW_H
