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
    QImage          *BUFdata;
    QString         path_;

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
    bool isNull() const;
    //    uint8_t* operator() (int x, int y);
    QImage* toImage(bool a = true, bool b = true, bool c = true);
    QImage* data();
    Format format();
    void flush();
};



#endif // IMGDATA_H
