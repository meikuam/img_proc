#include "imagelabel.h"

void ImageLabel::mousePressEvent(QMouseEvent *ev) {
//    this->setText("mousePressEvent");
    this->setCursor(Qt::ClosedHandCursor);
}
//void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
void ImageLabel::mouseReleaseEvent(QMouseEvent *ev) {
//    this->setText("mouseReleaseEvent");
    this->setCursor(Qt::OpenHandCursor);
}


void ImageLabel::onHorisontalSliderMoved(int value) {
//    this->setText("onHorisontalSliderMoved" + QString::number(value));
}

void ImageLabel::onVerticalSliderMoved(int value) {
//    this->setText("onVerticalSliderMoved" + QString::number(value));

}

void ImageLabel::setImage(QImage *image) {
    if(img != nullptr) delete img;
    img = image;
    dx = 0; dy = 0;
    scale = 1.0;
    w_win = this->width();
    h_win = this->height();
    repaint();
}

void ImageLabel::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        scale *= 1.1;
    } else {
        scale /= 1.1;
    }
    w_win = this->width();
    h_win = this->height();
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
//    this->setText("mouseMoveEvent" + QString::number(ev->x()) + " " + QString::number(ev->y()));
    dx += ev->x();
    dy += ev->y();
    w_win = this->width();
    h_win = this->height();
    repaint();

    this->verticalSliderMoved(ev->y());
    this->horisontalSliderMoved(ev->x());
}
 void ImageLabel::resizeEvent(QResizeEvent* event) {
     event->size();
     w_win = event->size().width();
     h_win = event->size().height();
     repaint();
 }


 void ImageLabel::repaint() {
     if(img != nullptr) {
         w_orig = img->width() * scale;
         h_orig = img->height() * scale;
         if (w_win < w_orig) {
             if (h_win < h_orig) { // 1

                 qDebug()<<"setImage: 1"<<endl;
                 x_pix = ((w_orig - w_win) / 2) - dx;
                 y_pix = ((h_orig - h_win) / 2) - dy;

                 if (x_pix < 0) {
                     x_pix = 0;
                 } else if (x_pix > w_orig - w_win) {
                     x_pix =  w_orig - w_win;
                 }
                 if (y_pix < 0) {
                     y_pix = 0;
                 } else if (y_pix > h_orig - h_win) {
                     y_pix = h_orig - h_win;
                 }

                 w_pix = w_orig - (w_orig - w_win);
                 h_pix = h_orig - (h_orig - h_win);

                 dx = 0; dy = 0;
                 horisontalSliderHide(false);
                 verticalSliderHide(false);
             } else { // 2

                 qDebug()<<"setImage: 2"<<endl;
                 y_pix = 0;
                 x_pix = ((w_orig - w_win) / 2) - dx;

                 if (x_pix < 0) {
                     x_pix = 0;
                 } else if (x_pix > w_orig - w_win) {
                     x_pix = w_orig - w_win;

                 }

                 w_pix = w_orig - (w_orig - w_win);
                 h_pix = h_orig;


                 dx = 0; dy = 0;

                 horisontalSliderHide(false);
                 verticalSliderHide(true);
             }
         } else {
             if (h_win < h_orig) { // 3

                 qDebug()<<"setImage: 3";
                 y_pix = ((h_orig - h_win) / 2) - dy;
                 x_pix = 0;

                 if (y_pix < 0) {
                     y_pix = 0;
                 } else if (y_pix > h_orig - h_win) {
                     y_pix = h_orig - h_win;
                 }
                 w_pix = w_orig;
                 h_pix = h_orig - (h_orig - h_win);

                 dx = 0; dy = 0;

                 horisontalSliderHide(true);
                 verticalSliderHide(false);
             } else { // 4

                 qDebug()<<"setImage: 4"<<endl;
                 x_pix = y_pix = 0;

                 w_pix = w_orig;
                 h_pix = h_orig;

                 dx = 0; dy = 0;

                 horisontalSliderHide(true);
                 verticalSliderHide(true);
             }
         }
         QPixmap p = QPixmap::fromImage(*img);
         QPixmap p_sub = p.copy(x_pix / scale, y_pix / scale, w_pix / scale, h_pix / scale);
         int w_sub = p_sub.width();
         int h_sub = p_sub.height();

         setPixmap(p_sub.scaled(w_sub * scale, h_sub * scale));
         //         setPixmap(p.scaled(w_orig, h_orig).copy(x_pix, y_pix, w_pix, h_pix));
     }


 }
