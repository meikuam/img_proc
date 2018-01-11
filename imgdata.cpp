#include "imgdata.h"


ImgData::ImgData(){
    RGBdata     = new QImage();
    BUFdata     = new QImage();
}
// create from path
ImgData::ImgData(QString path) {
    path_       = path;
    RGBdata     = new QImage(path);
    *RGBdata    = RGBdata->convertToFormat(QImage::Format_RGB888);

    BUFdata     = new QImage(RGBdata->copy());

    if(!RGBdata->isNull()) {
        format_     = Format_RGB;
    }
    std::cout<<"constructor ImgData"<<std::endl;
}
//    // create from image
//    ImgData(QImage img) {
//        *RGBdata        = img.copy();
//        *BUFdata        = img.copy();
//        format_         = Format_RGB;
//        std::cout<<"constructor ImgData"<<std::endl;
//    }
// copy constructor
ImgData::ImgData(const ImgData &imgdata) {
    //deep copy
    path_           = imgdata.path_;
    format_         = imgdata.format_;
    RGBdata         = new QImage(imgdata.RGBdata->copy());
    BUFdata         = new QImage(imgdata.BUFdata->copy());
    std::cout<<"copy ImgData"<<std::endl;
}

// assignment operator
ImgData& ImgData::operator = (const ImgData& imgdata) {
    //        delete  RGBdata; // untested
    path_           = imgdata.path_;
    format_         = imgdata.format_;
    RGBdata         = imgdata.RGBdata;
    BUFdata         = imgdata.BUFdata;
    std::cout<<"assignment ImgData  ImgData&"<<std::endl;
}

//    // assignment operator
//    ImgData& operator = (const QImage& img) {
////        format          = Format_RGB;
//        RGBdata         = &img;
//        std::cout<<"assignment ImgData QImage"<<std::endl;
//    }

ImgData::~ImgData() {
    delete RGBdata;
    delete BUFdata;
    std::cout<<"~ImgData()"<<std::endl;
}

void ImgData::load(QString path) {
    delete RGBdata;
    delete BUFdata;

    path_       = path;
    RGBdata     = new QImage(path);
    //        *RGBdata    = RGBdata->convertToFormat(QImage::Format_RGB888);
    BUFdata     = new QImage(RGBdata->copy());

    if(!RGBdata->isNull()) {
        format_     = Format_RGB;
    }
    std::cout<<"load ImgData"<<std::endl;
}

bool ImgData::isNull() const {
    //        std::cout<<"isNull() = "<<RGBdata->isNull()<<std::endl;
    return RGBdata->isNull();
}

//    uint8_t* operator() (int x, int y) {
//        RGB
//    }

// a, b, c are channels
QImage* ImgData::toImage(bool a, bool b, bool c) {
    if(!isNull()) {
        for(int x = 0; x < BUFdata->width(); x++) {
            for(int y = 0; y < BUFdata->height(); y++) {
                QRgb pix = RGBdata->pixel(x, y);
                int red         = a ? qRed(pix) : 0;
                int green       = b ?  qGreen(pix) : 0;
                int blue        = c ? qBlue(pix) : 0;
                BUFdata->setPixel(x, y, qRgb(red, green, blue));
            }
        }
        return BUFdata;
    } else {
        return nullptr;
    }
}

QImage* ImgData::data() {
    return RGBdata;
}

Format ImgData::format() {
    return format_;
}

void ImgData::flush() {
    if(!isNull()) {
        for(int x = 0; x < BUFdata->width(); x++) {
            for(int y = 0; y < BUFdata->height(); y++) {
                QRgb pix = RGBdata->pixel(x, y);
                int red         = qRed(pix);
                int green       = qGreen(pix);
                int blue        = qBlue(pix);
                BUFdata->setPixel(x, y, qRgb(red, green, blue));
            }
        }
    }
}
