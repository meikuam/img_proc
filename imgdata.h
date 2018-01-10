#ifndef IMGDATA_H
#define IMGDATA_H

#include <QImage>
#include <iostream>
enum Format {
    Format_RGB,
    Format_YCbCr,
    Format_HSV
};
using namespace std;

class ImgData{
    Format          format_;
    QImage          *RGBdata;
//    uint8_t         ***data;
public:

    ImgData(){
        RGBdata     = new QImage();
    }
    // create from path
    ImgData(QString path) {
        RGBdata     = new QImage(path);
        if(!RGBdata->isNull()) {
            format_     = Format_RGB;
        }
        std::cout<<"constructor ImgData"<<std::endl;
    }
    // create from image
    ImgData(QImage img) {
        *RGBdata        = img.copy();
        format_         = Format_RGB;
        std::cout<<"constructor ImgData"<<std::endl;
    }
    // copy constructor
    ImgData(const ImgData &imgdata) {
    //deep copy
        format_         = imgdata.format_;
        RGBdata         = new QImage(imgdata.RGBdata->copy());
        std::cout<<"copy ImgData"<<std::endl;
    }

    // assignment operator
    ImgData& operator = (const ImgData& imgdata) {
        delete  RGBdata; // untested
        format_         = imgdata.format_;
        RGBdata         = imgdata.RGBdata;
        std::cout<<"assignment ImgData  ImgData&"<<std::endl;
    }

//    // assignment operator
//    ImgData& operator = (const QImage& img) {
////        format          = Format_RGB;
//        RGBdata         = &img;
//        std::cout<<"assignment ImgData QImage"<<std::endl;
//    }

    ~ImgData() {
        delete RGBdata;
        std::cout<<"~ImgData()"<<std::endl;
    }

    bool isNull() const {
        std::cout<<"isNull() = "<<RGBdata->isNull()<<std::endl;
        return RGBdata->isNull();
    }

//    uint8_t* operator() (int x, int y) {
//        RGB
//    }

    // a, b, c are channels
    QImage* toImage(bool a = true, bool b = true, bool c = true) {
        QImage* img = new QImage(RGBdata->copy());
        if(a && b && c) {
            return img;
        }
        else {
            for(int x = 0; x < img->width(); x++) {
                for(int y = 0; y < img->height(); y++) {
                    QRgb pix = img->pixel(x, y);
                    int red         = a ? qRed(pix) : 0;
                    int green       = b ?  qGreen(pix) : 0;
                    int blue        = c ? qBlue(pix) : 0;
                    img->setPixel(x, y, qRgb(red, green, blue));
                }
            }
            return img;
        }
    }

    QImage* data() {
        return RGBdata;
    }

    Format format() {
        return format_;
    }
};



#endif // IMGDATA_H
