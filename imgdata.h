#ifndef IMGDATA_H
#define IMGDATA_H

#include <QImage>
#include <iostream>
#include "converter.h"

enum Format {
    Format_RGB,
    Format_YCbCr,
    Format_HSV
};
using namespace std;

//------------------Data2d--------------------
template<class Dtype>
class Data2d{
    int         w_,
                h_,
                c_;
    bool        nulldata_ = true;
    Dtype*      data_;
public:
    // constructor
    Data2d();
    Data2d(int w, int h, int c);
    Data2d(QImage* img);

    // copy constructor
    Data2d(const Data2d &data);
    // assignment operator
    Data2d& operator = (const Data2d& data);
    Dtype* operator()(int x, int y, int c);

    ~Data2d();

    inline bool        isNull() const {
        return nulldata_;
    }

    inline int         width() {
        return w_;
    }

    inline int         height() {
        return h_;
    }

    inline int         depth() {
        return c_;
    }
};

//------------------Data2d--------------------
//--------------implementation----------------
template<class Dtype>
Data2d<Dtype>::Data2d() {
    w_ = h_ = c_ = 0;
    nulldata_ = true;
}


template<class Dtype>
Data2d<Dtype>::Data2d(int w, int h, int c) {
    w_          = w;
    h_          = h;
    c_          = c;
    data_       = new Dtype[c_ * w_ * h_];
    nulldata_   = false;

}

//TODO: different data types are not supported. Need to check sizeof(QImage data)
template<class Dtype>
Data2d<Dtype>::Data2d(QImage* img) {
    if(img->isNull())
        return;
    w_          = img->width();
    h_          = img->height();
    c_          = img->depth() / (sizeof(Dtype) * 8);
    data_       = new Dtype[c_ * w_ * h_];
    for(int y = 0; y < h_; y++) {
        memcpy(&(data_[y * w_ * c_]), img->scanLine(y), w_ * c_);
    }
//    memcpy(data_, img->bits(), c_ * w_ * h_);
    nulldata_   = false;
}


//TODO: different data types are not supported
// copy constructor
template<class Dtype>
Data2d<Dtype>::Data2d(const Data2d<Dtype> &data) {
    w_          = data.w_;
    h_          = data.h_;
    c_          = data.c_;
    data_       = data.data_;
    nulldata_   = data.nulldata_;
}

//TODO: different data types are not supported
// assignment operator
template<class Dtype>
Data2d<Dtype>& Data2d<Dtype>::operator = (const Data2d<Dtype>& data) {
    w_          = data.w_;
    h_          = data.h_;
    c_          = data.c_;
    data_       = new Dtype[c_ * w_ * h_];
    memcpy(data_, data.data_, c_ * w_ * h_);
    nulldata_   = data.nulldata_;
}


template<class Dtype>
Dtype* Data2d<Dtype>::operator()(int x, int y, int c){
    // It is assumed that the data is stored as follows:
    //
    //     <- w_ ->
    // ^  abc abc abc
    // h_ abc abc abc
    // v  abc abc abc
    //
    //   abc - c_ elements

    return &(data_[(y * w_ + x) * c_ + c]);
}

template<class Dtype>
Data2d<Dtype>::~Data2d() {
    nulldata_   = true;
    delete data_;
}


//-----------------ImgData-------------------
class ImgData{
    Format          format_;
    //TODO: Datatype is hardcoded
    Data2d<uint8_t> *data_;
    QImage          *RGBdata_;
    QString         path_;
    //TODO: Channel num is hardcoded
    bool            channel_[3] = {true, true, true};

    inline uint8_t* data(int x, int y, int c) {
        return (*data_)(x, y, c);
    }

public:
    ImgData();
    // create from path
    ImgData(QString path);
    //    // create from image
    //    ImgData(QImage img);
    // copy constructor
    ImgData(const ImgData &imgdata);
    // assignment operator
    ImgData& operator = (const ImgData& imgdata);
    //    // assignment operator
    //    ImgData& operator = (const QImage& img);
    ~ImgData();

    void load(QString path);

    inline bool isNull() const {
        return (RGBdata_->isNull() || data_->isNull());
    }
    inline Format format() {
        return format_;
    }
    inline uint8_t* operator() (int x, int y, int c) {
        return (*data_)(x, y, c);
    }

    inline int         width() {
        return data_->width();
    }

    inline int         height() {
        return data_->height();
    }

    inline int         depth() {
        return data_->depth();
    }

    QImage* img();

    //TODO: Channel num is hardcoded
    inline void setChannels(bool a = true, bool b = true, bool c = true) {
        //TODO: It is assumed that there are three channels
        channel_[0] = a;
        channel_[1] = b;
        channel_[2] = c;
    }

    void RGBupdate();
    void convertTo(Format f);
};

#endif // IMGDATA_H
