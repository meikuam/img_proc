#include "imgdata.h"


//-----------------ImgData-------------------
//--------------constructors-----------------

ImgData::ImgData(){
    data_       = new Data2d<uint8_t>();
    RGBdata_    = new QImage();
}

// create from path
ImgData::ImgData(QString path) {
    path_       = path;
    RGBdata_    = new QImage(path_);
    if(RGBdata_->isNull()) {
        data_       = new Data2d<uint8_t>();
    } else {
        *RGBdata_   = RGBdata_->convertToFormat(QImage::Format_RGB888);
        data_       = new Data2d<uint8_t>(RGBdata_);
        format_     = Format_RGB;
        name_       = path.section("/",-1,-1);
    }
}

//    // create from image
//    ImgData(QImage img) {
//        *RGBdata        = img.copy();
//        *BUFdata        = img.copy();
//        format_         = Format_RGB;
//        std::cout<<"constructor ImgData"<<std::endl;
//    }

//---------copy constructor-----------------
ImgData::ImgData(const ImgData &imgdata) {
    //deep copy
    path_           = imgdata.path_;
    format_         = imgdata.format_;

    name_           = imgdata.name_;
    RGBdata_        = new QImage(imgdata.RGBdata_->copy());
    data_           = new Data2d<uint8_t>(*(imgdata.data_));
}


//---------assignment operator--------------
ImgData& ImgData::operator = (const ImgData& imgdata) {
    path_           = imgdata.path_;
    format_         = imgdata.format_;

    name_           = imgdata.name_;
    RGBdata_        = imgdata.RGBdata_;
    data_           = imgdata.data_;
}

//    // assignment operator
//    ImgData& operator = (const QImage& img) {
//        RGBdata         = &img;
//    }

//--------------destructor-------------------
ImgData::~ImgData() {
    delete RGBdata_;
    delete data_;
}

//--------------class methods----------------
void ImgData::load(QString path) {
    delete RGBdata_;
    delete data_;

    path_       = path;
    RGBdata_    = new QImage(path);
    if(RGBdata_->isNull()) {
        data_       = new Data2d<uint8_t>();
    } else {
        *RGBdata_   = RGBdata_->convertToFormat(QImage::Format_RGB888);
        data_       = new Data2d<uint8_t>(RGBdata_);
        format_     = Format_RGB;
        name_       = path.section("/",-1,-1);
    }
}

QImage* ImgData::img() {
    if(!isNull()) {
        RGBupdate();
        return RGBdata_;
    } else {
        return nullptr;
    }
}

void ImgData::RGBupdate() {
    //TODO: It is assumed that there are three channels
    if(!isNull()) {
        switch(format_) {
        case Format_RGB: {
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    if(channel_[0] && !channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 0),
                                                      *data(x, y, 0),
                                                      *data(x, y, 0)));
                    } else if(!channel_[0] && channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 1),
                                                      *data(x, y, 1),
                                                      *data(x, y, 1)));
                    } else if(!channel_[0] && !channel_[1] && channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 2),
                                                      *data(x, y, 2),
                                                      *data(x, y, 2)));
                    } else {
                        int red         = channel_[0] ? *data(x, y, 0) : 0;
                        int green       = channel_[1] ? *data(x, y, 1) : 0;
                        int blue        = channel_[2] ? *data(x, y, 2) : 0;
                        RGBdata_->setPixel(x, y, qRgb(red, green, blue));
                    }
                }
            }
            break;
        }
        case Format_HSV: {
            uint8_t* hsv = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    if(channel_[0] && !channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 0),
                                                      *data(x, y, 0),
                                                      *data(x, y, 0)));
                    } else if(!channel_[0] && channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 1),
                                                      *data(x, y, 1),
                                                      *data(x, y, 1)));
                    } else if(!channel_[0] && !channel_[1] && channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 2),
                                                      *data(x, y, 2),
                                                      *data(x, y, 2)));
                    } else {
                        hsv[0]              = *data(x, y, 0);
                        hsv[1]              = *data(x, y, 1);
                        hsv[2]              = *data(x, y, 2);
                        Converter::HSV2RGB(hsv, rgb, channel_[0], channel_[1], channel_[2]);
                        RGBdata_->setPixel(x, y, qRgb(rgb[0], rgb[1], rgb[2]));
                    }

                }
            }
            delete hsv;
            delete rgb;
            break;
        }
        case Format_YCbCr: {
            uint8_t* ycbcr = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    if(channel_[0] && !channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 0),
                                                      *data(x, y, 0),
                                                      *data(x, y, 0)));
                    } else if(!channel_[0] && channel_[1] && !channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 1),
                                                      *data(x, y, 1),
                                                      *data(x, y, 1)));
                    } else if(!channel_[0] && !channel_[1] && channel_[2]) {
                        RGBdata_->setPixel(x, y, qRgb(*data(x, y, 2),
                                                      *data(x, y, 2),
                                                      *data(x, y, 2)));
                    } else {
                        ycbcr[0]            = *data(x, y, 0);
                        ycbcr[1]            = *data(x, y, 1);
                        ycbcr[2]            = *data(x, y, 2);
                        Converter::YCbCr2RGB(ycbcr, rgb, channel_[0], channel_[1], channel_[2]);
                        RGBdata_->setPixel(x, y, qRgb(rgb[0], rgb[1], rgb[2]));
                    }
                }
            }
            delete ycbcr;
            delete rgb;
            break;
        }
        }
    }
}


void ImgData::convertTo(Format f) {
    //TODO: It is assumed that there are three channels
    switch (format_) {
    case Format_RGB:
        switch(f) {
        case Format_RGB: {
            // RGB -> RGB
            break;
        }
        case Format_HSV: {
            // RGB -> HSV
            uint8_t* hsv = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    rgb[0]              = *data(x, y, 0);
                    rgb[1]              = *data(x, y, 1);
                    rgb[2]              = *data(x, y, 2);
                    Converter::RGB2HSV(rgb, hsv);
                    *data(x, y, 0)      = hsv[0];
                    *data(x, y, 1)      = hsv[1];
                    *data(x, y, 2)      = hsv[2];
                }
            }
            delete rgb;
            delete hsv;
            format_ = Format_HSV;
            break;
        }
        case Format_YCbCr: {
            // RGB -> YCbCr
            uint8_t* ycbcr = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    rgb[0]              = *data(x, y, 0);
                    rgb[1]              = *data(x, y, 1);
                    rgb[2]              = *data(x, y, 2);
                    Converter::RGB2YCbCr(rgb, ycbcr);
                    *data(x, y, 0)      = ycbcr[0];
                    *data(x, y, 1)      = ycbcr[1];
                    *data(x, y, 2)      = ycbcr[2];
                }
            }
            delete rgb;
            delete ycbcr;
            format_ = Format_YCbCr;
            break;
        }
        }
        break;
    case Format_HSV:
        switch(f) {
        case Format_RGB: {
            // HSV -> RGB
            uint8_t* hsv = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    hsv[0]              = *data(x, y, 0);
                    hsv[1]              = *data(x, y, 1);
                    hsv[2]              = *data(x, y, 2);
                    Converter::HSV2RGB(hsv, rgb);
                    *data(x, y, 0)      = rgb[0];
                    *data(x, y, 1)      = rgb[1];
                    *data(x, y, 2)      = rgb[2];
                }
            }
            delete hsv;
            delete rgb;
            format_ = Format_RGB;
            break;
        }
        case Format_HSV: {
            // HSV -> HSV

            break;
        }
        case Format_YCbCr: {
            // HSV -> RGB -> YCbCr
            uint8_t* ycbcr = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            uint8_t* hsv = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    hsv[0]              = *data(x, y, 0);
                    hsv[1]              = *data(x, y, 1);
                    hsv[2]              = *data(x, y, 2);
                    Converter::HSV2RGB(hsv, rgb);
                    Converter::RGB2YCbCr(rgb, ycbcr);
                    *data(x, y, 0)      = ycbcr[0];
                    *data(x, y, 1)      = ycbcr[1];
                    *data(x, y, 2)      = ycbcr[2];

                }
            }
            delete rgb;
            delete hsv;
            delete ycbcr;
            format_ = Format_YCbCr;
            break;
        }
        }
        break;
    case Format_YCbCr:
        switch(f) {
        case Format_RGB:
        {
            uint8_t* ycbcr = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    ycbcr[0]            = *data(x, y, 0);
                    ycbcr[1]            = *data(x, y, 1);
                    ycbcr[2]            = *data(x, y, 2);
                    Converter::YCbCr2RGB(ycbcr, rgb);
                    *data(x, y, 0)      = rgb[0];
                    *data(x, y, 1)      = rgb[1];
                    *data(x, y, 2)      = rgb[2];
                }
            }
            delete rgb;
            delete ycbcr;
            format_ = Format_RGB;
            break;
        }
        case Format_HSV: {
            // YCbCr -> RGB -> HSV
            uint8_t* ycbcr = new uint8_t[3];
            uint8_t* rgb = new uint8_t[3];
            uint8_t* hsv = new uint8_t[3];
            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    ycbcr[0]            = *data(x, y, 0);
                    ycbcr[1]            = *data(x, y, 1);
                    ycbcr[2]            = *data(x, y, 2);
                    Converter::YCbCr2RGB(ycbcr, rgb);
                    Converter::RGB2HSV(rgb, hsv);
                    *data(x, y, 0)      = hsv[0];
                    *data(x, y, 1)      = hsv[1];
                    *data(x, y, 2)      = hsv[2];
                }
            }
            delete rgb;
            delete hsv;
            delete ycbcr;
            format_ = Format_HSV;
            break;
        }
        case Format_YCbCr: {
            // YCbCr -> YCbCr
            break;
        }
        }
    }
    setChannels();
    RGBupdate();
}
