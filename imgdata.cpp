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

    RGBdata_        = new QImage(imgdata.RGBdata_->copy());
    data_           = new Data2d<uint8_t>(RGBdata_);
}


//---------assignment operator--------------
ImgData& ImgData::operator = (const ImgData& imgdata) {
    path_           = imgdata.path_;
    format_         = imgdata.format_;

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
                    int red         = channel_[0] ? *data(x, y, 0) : 0;
                    int green       = channel_[1] ? *data(x, y, 1) : 0;
                    int blue        = channel_[2] ? *data(x, y, 2) : 0;
                    RGBdata_->setPixel(x, y, qRgb(red, green, blue));
                }
            }
            break;
        }
        case Format_HSV: {
            break;
        }
        case Format_YCbCr: {
            double  M[3][3] = {{1.0,     0.0,        1.402},
                               {1.0,     -0.344,     -0.714},
                               {1.0,     1.722,      0.0}};
            int     RGB[3];
            double  YCbCr[3];

            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    YCbCr[0]        = channel_[0] ? *data(x, y, 0)         : 0;
                    YCbCr[1]        = channel_[1] ? *data(x, y, 1) - 128.0 : 0;
                    YCbCr[2]        = channel_[2] ? *data(x, y, 2) - 128.0 : 0;

                    RGB[0]          = YCbCr[0] * M[0][0] + YCbCr[1] * M[0][1] + YCbCr[2] * M[0][2];
                    RGB[1]          = YCbCr[0] * M[1][0] + YCbCr[1] * M[1][1] + YCbCr[2] * M[1][2];
                    RGB[2]          = YCbCr[0] * M[2][0] + YCbCr[1] * M[2][1] + YCbCr[2] * M[2][2];

                    RGB[0]          = RGB[0] > 255 ? 255 : RGB[0] < 0 ? 0 : RGB[0];
                    RGB[1]          = RGB[1] > 255 ? 255 : RGB[1] < 0 ? 0 : RGB[1];
                    RGB[2]          = RGB[2] > 255 ? 255 : RGB[2] < 0 ? 0 : RGB[2];

                    RGBdata_->setPixel(x, y, qRgb(RGB[0], RGB[1], RGB[2]));
                }
            }
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

            format_ = Format_HSV;
            break;
        }
        case Format_YCbCr: {
            // RGB -> YCbCr
            int     YCbCr[3];
            double  M[3][3] = {{0.299,     0.587,        0.114},
                               {-0.169,    -0.331,       0.500},
                               {0.500,     -0.419,      -0.081}};
            double  RGB[3];

            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {

                    RGB[0]          = *data(x, y, 0);
                    RGB[1]          = *data(x, y, 1);
                    RGB[2]          = *data(x, y, 2);

                    YCbCr[0]        = RGB[0] * M[0][0] + RGB[1] * M[0][1] + RGB[2] * M[0][2];
                    YCbCr[1]        = RGB[0] * M[1][0] + RGB[1] * M[1][1] + RGB[2] * M[1][2] + 128.0;
                    YCbCr[2]        = RGB[0] * M[2][0] + RGB[1] * M[2][1] + RGB[2] * M[2][2] + 128.0;

                    *data(x, y, 0)   = YCbCr[0] > 255 ? 255 : YCbCr[0] < 0 ? 0 : YCbCr[0];
                    *data(x, y, 1)   = YCbCr[1] > 255 ? 255 : YCbCr[1] < 0 ? 0 : YCbCr[1];
                    *data(x, y, 2)   = YCbCr[2] > 255 ? 255 : YCbCr[2] < 0 ? 0 : YCbCr[2];
                }
            }
            format_ = Format_YCbCr;
            break;
        }
        }

        break;
    case Format_HSV:
        switch(f) {
        case Format_RGB: {
            // HSV -> RGB

            break;
        }
        case Format_HSV: {
            // HSV -> HSV

            break;
        }
        case Format_YCbCr: {
            // HSV -> RGB -> YCbCr

            break;
        }
        }
        break;
    case Format_YCbCr:
        switch(f) {
        case Format_RGB:
        {
            double  M[3][3] = {{1.0,     0.0,        1.402},
                               {1.0,     -0.344,     -0.714},
                               {1.0,     1.722,      0.0}};
            int     RGB[3];
            double  YCbCr[3];

            for(int x = 0; x < data_->width(); x++) {
                for(int y = 0; y < data_->height(); y++) {
                    YCbCr[0]        = *data(x, y, 0);
                    YCbCr[1]        = *data(x, y, 1) - 128.0;
                    YCbCr[2]        = *data(x, y, 2) - 128.0;

                    RGB[0]          = YCbCr[0] * M[0][0] + YCbCr[1] * M[0][1] + YCbCr[2] * M[0][2];
                    RGB[1]          = YCbCr[0] * M[1][0] + YCbCr[1] * M[1][1] + YCbCr[2] * M[1][2];
                    RGB[2]          = YCbCr[0] * M[2][0] + YCbCr[1] * M[2][1] + YCbCr[2] * M[2][2];

                    *data(x, y, 0)   = RGB[0] > 255 ? 255 : RGB[0] < 0 ? 0 : RGB[0];
                    *data(x, y, 1)   = RGB[1] > 255 ? 255 : RGB[1] < 0 ? 0 : RGB[1];
                    *data(x, y, 2)   = RGB[2] > 255 ? 255 : RGB[2] < 0 ? 0 : RGB[2];
                }
            }
            format_ = Format_RGB;
            break;
        }
        case Format_HSV: {
            // YCbCr -> RGB -> HSV
            break;
        }
        case Format_YCbCr: {
            // YCbCr -> YCbCr
            break;
        }
        }
    }
    RGBupdate();
}
