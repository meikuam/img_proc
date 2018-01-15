#ifndef FILTER_H
#define FILTER_H

#include <iostream>

#include "data2d.h"
#include "imgdata.h"

using namespace std;


enum Method {
    Sobel,          //Собеля
    Morphology,     //Математической морфологии
    Prewitt,        //Превитта
    Canny,          //Канни
    Laplas,         //Лапласа
    Roberts,        //Робертса
    Binary
};

class Mask {
    int w_, h_, dims_;
    double* m_;
public:
    Mask(Method method);
    ~Mask();

    inline double operator()(int x, int y, int dim) {
        return m_[dim * w_ * h_ + y * h_ + x];
    }

    inline int width() {
        return w_;
    }

    inline int height() {
        return h_;
    }

    inline int dims() {
        return dims_;
    }
};


class Filter {

public:
//    static void filter(Data2d<uint8_t>* src,
//                       Data2d<uint8_t>* dst,
//                       Method method);
    static void filter(ImgData* src,
                       ImgData* dst,
                       Method method);
};

#endif // FILTER_H
