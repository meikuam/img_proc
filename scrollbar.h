#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QtWidgets>
#include <qscrollbar.h>
#include <QDebug>
namespace Ui {
    class ScrollBar;
}

class ScrollBar : public QScrollBar{
    Q_OBJECT

public:
    explicit ScrollBar(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags()) :
        QScrollBar(parent){}
    explicit ScrollBar(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR) :
        QScrollBar(orientation, parent) {}

    Q_SLOT
    void Hide(bool hide);
    Q_SLOT
    void Maximum(int max);

//    Q_SLOT
//    void onHorisontalSliderMoved(int value);
//    Q_SLOT
//    void onVerticalSliderMoved(int value);

private:

};
#endif // SCROLLBAR_H
