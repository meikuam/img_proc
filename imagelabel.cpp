#include "imagelabel.h"

void ImageLabel::mousePressEvent(QMouseEvent *ev) {
    this->setText("mousePressEvent");
    this->setCursor(Qt::ClosedHandCursor);
}
//void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
void ImageLabel::mouseReleaseEvent(QMouseEvent *ev) {
    this->setText("mouseReleaseEvent");
    this->setCursor(Qt::OpenHandCursor);
}

void ImageLabel::wheelEvent(QWheelEvent *event) {
    qDebug()<<"wheelEvent: " + QString::number(event->delta()) + " "+ QString::number(this->width())+ " "+ QString::number(this->height());

    if(event->delta() > 0) {
        scale *= 1.1;
    } else {
        scale /= 1.1;
    }
    int w = img.width() * scale,
        h = img.height() * scale;

    QPixmap p = QPixmap::fromImage(img);

    int x_pix, w_pix,
        y_pix, h_pix;
    if(w > this->width()) {
        x_pix = (w - this->width() - 1) / 2;
        w_pix = this->width();
    } else {
        w_pix = w;
        x_pix = 0;
    }
    if(h > this->width()) {
        y_pix = (h - this->height() - 1) / 2;
        h_pix = this->height();
    } else {
        h_pix = h;
        y_pix = 0;
    }
    setPixmap(p.scaled(w, h).copy(x_pix, y_pix, w_pix, h_pix));
//    this->setPixmap();
}

void ImageLabel::mouseMoveEvent(QMouseEvent *ev) {
    this->setText("mouseMoveEvent" + QString::number(ev->x()) + " " + QString::number(ev->y()));

    this->verticalSliderMoved(ev->y());
    this->horisontalSliderMoved(ev->x());
}

void ImageLabel::onHorisontalSliderMoved(int value) {
    this->setText("onHorisontalSliderMoved" + QString::number(value));
}

void ImageLabel::onVerticalSliderMoved(int value) {
    this->setText("onVerticalSliderMoved" + QString::number(value));

}

void ImageLabel::setImage(QImage &image) {
    img = image;

    int w = img.width() * scale,
        h = img.height() * scale;

    QPixmap p = QPixmap::fromImage(img);

    int x_pix, w_pix,
        y_pix, h_pix;
    if(w > this->width()) {
        x_pix = (w - this->width() - 1) / 2;
        w_pix = this->width() - 1;
    } else {
        w_pix = w;
        x_pix = 0;
    }
    if(h > this->width()) {
        y_pix = (h - this->height() - 1) / 2;
        h_pix = this->height() - 1;
    } else {
        h_pix = h;
        y_pix = 0;
    }
    setPixmap(p.scaled(w, h).copy(x_pix, y_pix, w_pix, h_pix));
}

// void ImageLabel::resizeEvent(QResizeEvent* event) {
//     int w = img.width() * scale,
//         h = img.height() * scale;

//     QPixmap p = QPixmap::fromImage(img);

//     int x_pix, w_pix,
//         y_pix, h_pix;
//     if(w > this->width()) {
//         x_pix = (w - this->width() - 1) / 2;
//         w_pix = this->width() - 1;
//     } else {
//         w_pix = w;
//         x_pix = 0;
//     }
//     if(h > this->width()) {
//         y_pix = (h - this->height() - 1) / 2;
//         h_pix = this->height() - 1;
//     } else {
//         h_pix = h;
//         y_pix = 0;
//     }
//     setPixmap(p.scaled(w, h).copy(x_pix, y_pix, w_pix, h_pix));
// }
