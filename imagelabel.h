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
        QLabel(parent, f){}
    explicit ImageLabel(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags()) :
        QLabel(text, parent, f) {}

    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void wheelEvent ( QWheelEvent * event );
    Q_SLOT
    void onHorisontalSliderMoved(int value);
    Q_SLOT
    void onVerticalSliderMoved(int value);

//    Q_SLOT
//    void resizeEvent(QResizeEvent* event);

    Q_SIGNAL
    void verticalSliderMoved(int value);
    Q_SIGNAL
    void horisontalSliderMoved(int value);

    void setImage(QImage &img);
private:
    QImage img;
    double scale = 1.0;
    int left_up_x = 0,
        left_ip_y = 0;
//     void paintEvent(QPaintEvent *)
};
#endif // IMAGELABEL_H