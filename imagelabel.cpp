#include "imagelabel.h"

void ImageLabel::mousePressEvent(QMouseEvent *ev) {
    this->setText("mousePressEvent");
    this->setCursor(Qt::ClosedHandCursor);
}
//void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
void ImageLabel::mouseReleaseEvent(QMouseEvent *ev) {
//    this->setText("mouseReleaseEvent");
    this->setCursor(Qt::OpenHandCursor);
}

void ImageLabel::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        scale *= 1.1;
    } else {
        scale /= 1.1;
    }
    repaint();
    qDebug()<<"wheelEvent:"<<endl
           <<"w_win\t\t"<<w_win<<endl
           <<"h_win\t\t"<<h_win<<endl
           <<"w_orig\t\t"<<w_orig<<endl
           <<"h_orig\t\t"<<h_orig<<endl
           <<"x_pix\t\t"<<x_pix<<endl
           <<"y_pix\t\t"<<y_pix<<endl
           <<"w_pix\t\t"<<w_pix<<endl
           <<"h_pix\t\t"<<h_pix<<endl;
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
    repaint();
}

 void ImageLabel::resizeEvent(QResizeEvent* event) {
     repaint();
 }


 void ImageLabel::repaint() {
     if(!img.isNull()) {
         w_orig = img.width() * scale;
         h_orig = img.height() * scale;
         w_win = this->width();
         h_win = this->height();
         if (w_win < w_orig) {
             if (h_win < h_orig) { // 1

                 qDebug()<<"setImage: 1"<<endl;
                 x_pix = (w_orig - w_win) / 2;
                 y_pix = (h_orig - h_win) / 2;

                 w_pix = w_orig - (w_orig - w_win);
                 h_pix = h_orig - (h_orig - h_win);
             } else { // 2

                 qDebug()<<"setImage: 2"<<endl;
                 y_pix = 0;
                 x_pix = (w_orig - w_win) / 2;

                 w_pix = w_orig - (w_orig - w_win);
                 h_pix = h_orig;
             }
         } else {
             if (h_win < h_orig) { // 3

                 qDebug()<<"setImage: 3";
                 y_pix = (h_orig - h_win) / 2;
                 x_pix = 0;

                 w_pix = w_orig;
                 h_pix = h_orig - (h_orig - h_win);

             } else { // 4

                 qDebug()<<"setImage: 4"<<endl;
                 x_pix = y_pix = 0;

                 w_pix = w_orig;
                 h_pix = h_orig;
             }
         }
         QPixmap p = QPixmap::fromImage(img);
         setPixmap(p.scaled(w_orig, h_orig).copy(x_pix, y_pix, w_pix, h_pix));
     }

 }
