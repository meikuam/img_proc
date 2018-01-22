#ifndef FILTER_H
#define FILTER_H

#include <iostream>

#include "data2d.h"
#include "imgdata.h"

using namespace std;


enum Method {
    Sobel,          //0 Собеля
    Morphology,     //1 Математической морфологии
    Prewitt,        //2 Превитта
    Canny,          //3 Канни
    Laplas,         //4 Лапласа
    Roberts,        //5 Робертса
    Binary,         //6
    Gauss,          //7
    Gray,           //8

    //morphological operations are implemented in morph.h/cpp
    Erosion,
    Dilation,
    Subtraction,
    Inversion,
    HitOrMiss,
    Union,
    Intersection
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
