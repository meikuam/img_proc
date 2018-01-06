#ifndef IMGDATA_H
#define IMGDATA_H

#include <QImage>

enum Format {
    Format_RGB,
    Format_YCbCr,
    Format_HSV
};


class ImgData{
public:
    Format format = Format_RGB;
    QImage *img;
    bool channels[3];
    ImgData(QImage *ptr){
        img = ptr;
        channels[0] = channels[1] = channels[2] = true;
    }
    QImage* turnOffChannel(int ch) {
        QImage* buf = new QImage(img->copy());
        if(channels[ch]) {
            channels[ch] = false;
            for(int x = 0; x < buf->width(); x++) {
                for(int y = 0; y < buf->height(); y++) {
                    QRgb pix = buf->pixel(x, y);
                    int red = channels[0] ? qRed(pix) : 0;
                    int green = channels[1] ?  qGreen(pix) : 0;
                    int blue = channels[2] ? qBlue(pix) : 0;
                    buf->setPixel(x, y, qRgb(red, green, blue));
                }
            }
        }
        return buf;
    }
};



#endif // IMGDATA_H
