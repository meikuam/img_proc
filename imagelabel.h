#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QtWidgets>
#include <qlabel.h>
#include <QDebug>
namespace Ui {
    class ImageLabel;
}

class ImageLabel : public QLabel{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags()) :
        QLabel(parent, f){ img = nullptr;}
    explicit ImageLabel(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags()) :
        QLabel(text, parent, f) {img = nullptr;}

    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void wheelEvent ( QWheelEvent * event );
    Q_SLOT
    void onHorisontalSliderMoved(int value);
    Q_SLOT
    void onVerticalSliderMoved(int value);

    Q_SLOT
    void resizeEvent(QResizeEvent* event);

    Q_SIGNAL
    void verticalSliderMoved(int value);
    Q_SIGNAL
    void horisontalSliderMoved(int value);


    Q_SIGNAL
    void verticalSliderMaximum(int value);
    Q_SIGNAL
    void horisontalSliderMaximum(int value);

    Q_SIGNAL
    void verticalSliderHide(bool);
    Q_SIGNAL
    void horisontalSliderHide(bool);

    void setImage(QImage *img);
    void repaint();

private:

    QImage* img;
    double scale = 1.0;
    int dx, dy;
    int x_pix, w_pix,
        y_pix, h_pix;

    int w_orig,
        h_orig;
    int w_win,
        h_win;
//     void paintEvent(QPaintEvent *)
};
#endif // IMAGELABEL_H
