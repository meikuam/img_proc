#ifndef IMGDATA_H
#define IMGDATA_H

#include <QImage>
#include <iostream>
#include "converter.h"
#include "data2d.h"

enum Format {
    Format_RGB,
    Format_YCbCr,
    Format_HSV
};
using namespace std;

//-----------------ImgData-------------------
class ImgData{
    Format          format_;
    //TODO: Datatype is hardcoded
    Data2d<uint8_t> *data_;
    QImage          *RGBdata_;
    QString         path_,
                    name_;
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
    inline void setName(QString name) {
        name_ = name;
    }

    inline QString getName() {
        return name_;
    }
};

#endif // IMGDATA_H
