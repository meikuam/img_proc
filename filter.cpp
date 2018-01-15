#include "filter.h"
#include <math.h>


Mask::Mask(Method method) {
    switch (method) {
    case Sobel:
    {
        w_ = 3; h_ = 3; dims_ = 2;
        m_ = new double[w_ * h_ * dims_] { -1.0,  0.0,    1.0,
                                           -2.0,  0.0,    2.0,
                                           -1.0,  0.0,    1.0,

                                           1.0,   2.0,    1.0,
                                           0.0,   0.0,    0.0,
                                           -1.0,  -2.0,  -1.0};

//        m_[0] = -1.0;       m_[2] = 0.0;        m_[4] = 1.0;
//        m_[6] = -2.0;       m_[8] = 0.0;        m_[10] = 2.0;
//        m_[12] = -1.0;      m_[14] = 0.0;       m_[16] = 1.0;

//        m_[1] = 1.0;        m_[3] = 2.0;        m_[5] = 1.0;
//        m_[7] = 0.0;        m_[9] = 0.0;        m_[11] = 0.0;
//        m_[13] = -1.0;      m_[15] = -2.0;      m_[17] = -1.0;
        break;
    }
    case Morphology:
    {
        break;
    }
    case Prewitt:
    {
        w_ = 3; h_ = 3; dims_ = 2;
        m_ = new double[w_ * h_ * dims_] { -1.0,  0.0,    1.0,
                                           -1.0,  0.0,    1.0,
                                           -1.0,  0.0,    1.0,

                                           1.0,   1.0,    1.0,
                                           0.0,   0.0,    0.0,
                                           -1.0,  -1.0,  -1.0};
        break;
    }
    case Canny:
    {
        break;
    }
    case Laplas:
    {

        break;
    }
    case Roberts:
    {
        break;
    }
    }
}

Mask::~Mask() {
    delete m_;
}
void Filter::filter(ImgData *src,
                    ImgData *dst,
                    Method method) {
//    Sobel,          //Собеля
//    Morphology,     //Математической морфологии
//    Prewitt,        //Превитта
//    Canny,          //Канни
//    Laplas,         //Лапласа
//    Roberts         //Робертса
    switch (method) {
    case Sobel:
    {
        Mask m(Sobel);

        for(int x = 1; x < src->width() - 1; x++) {
            for(int y = 1; y < src->height() - 1; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int c = 0; c < src->depth(); c++) {
                    for(int s = -1; s <= 1; s++) {
                        for(int t = -1; t <= 1; t++) {
                            gx += (*(*src)(x + s, y + t, c)) * m(1 + s, 1 + t, 0);
                            gy += (*(*src)(x + s, y + t, c)) * m(1 + s, 1 + t, 1);
                        }
                    }
                }
                uint8_t res = sqrt(gx * gx + gy * gy);
                *((*dst)(x, y, 0)) = res;
                *((*dst)(x, y, 1)) = res;
                *((*dst)(x, y, 2)) = res;
            }
        }
        break;
    }
    case Morphology:
    {
        break;
    }
    case Prewitt:
    {
        Mask m(Prewitt);

        for(int x = 1; x < src->width() - 1; x++) {
            for(int y = 1; y < src->height() - 1; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int c = 0; c < src->depth(); c++) {
                    for(int s = -1; s <= 1; s++) {
                        for(int t = -1; t <= 1; t++) {
                            gx += (*(*src)(x + s, y + t, c)) * m(1 + s, 1 + t, 0);
                            gy += (*(*src)(x + s, y + t, c)) * m(1 + s, 1 + t, 1);
                        }
                    }
                }
                uint8_t res = sqrt(gx * gx + gy * gy);
                *((*dst)(x, y, 0)) = res;
                *((*dst)(x, y, 1)) = res;
                *((*dst)(x, y, 2)) = res;
            }
        }
        break;
    }
    case Canny:
    {
        break;
    }
    case Laplas:
    {

        break;
    }
    case Roberts:
    {
        break;
    }
    case Binary:
    {
        uint8_t t = 127;

        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                for(int c = 0; c < src->depth(); c++) {
                    *((*dst)(x, y, 0)) = (*(*src)(x, y, 0)) >= t ? 255 : 0;
                }
            }
        }
        break;
    }
    }
}
