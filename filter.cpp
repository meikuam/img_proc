#include "filter.h"
#include <math.h>


Mask::Mask(Method method) {
    switch (method) {
    case Sobel: //0
    {
        w_ = 3; h_ = 3; dims_ = 2;
        m_ = new double[w_ * h_ * dims_] { -1.0,  0.0,    1.0,
                                           -2.0,  0.0,    2.0,
                                           -1.0,  0.0,    1.0,

                                           1.0,   2.0,    1.0,
                                           0.0,   0.0,    0.0,
                                           -1.0,  -2.0,  -1.0};
        break;
    }
    case Morphology: //1
    {
        w_ = 5; h_ = 5; dims_ = 1;
        m_ = new double[w_ * h_ * dims_] { 0.0,   0.0,    1.0,    0.0,  0.0,
                                           0.0,   1.0,    1.0,    1.0,  0.0,
                                           1.0,   1.0,    1.0,    1.0,  1.0,
                                           0.0,   1.0,    1.0,    1.0,  0.0,
                                           0.0,   0.0,    1.0,    0.0,  0.0};
        break;
    }
    case Prewitt: //2
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
    case Canny: //3
    {
        break;
    }
    case Laplas: //4
    {

        break;
    }
    case Roberts: //5
    {
        break;
    }
    case Binary: //6
    {
        break;
    }
    case Gauss: //7
    {

        w_ = 5; h_ = 5; dims_ = 1;
        //Canny mask
        m_ = new double[w_ * h_ * dims_] { 2.0,   4.0,    5.0,    4.0,  2.0,
                                           4.0,   9.0,   12.0,    9.0,  4.0,
                                           5.0,   12.0,  15.0,    12.0, 5.0,
                                           4.0,   9.0,   12.0,    9.0,  4.0,
                                           2.0,   4.0,    5.0,    4.0,  2.0};
        for(int i = 0; i < w_*h_*dims_; i++) {
            m_[i] /= 159.0;
        }
//        m_ = new double[w_ * h_ * dims_] { 1.0,   4.0,    7.0,    4.0,  1.0,
//                                           4.0,   16.0,  26.0,    16.0, 4.0,
//                                           7.0,   26.0,  41.0,    26.0, 7.0,
//                                           4.0,   16.0,  26.0,    16.0, 4.0,
//                                           1.0,   4.0,    7.0,    4.0,  1.0};
//        for(int i = 0; i < w_*h_*dims_; i++) {
//            m_[i] /= 273.0;
//        }
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
    case Sobel: //0
    {
        Mask m(Sobel);

        for(int x = 1; x < src->width() - 1; x++) {
            for(int y = 1; y < src->height() - 1; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int c = 0; c < src->depth(); c++) {
                    for(int s = -1; s <= 1; s++) {
                        for(int t = -1; t <= 1; t++) {
                            gx += (*(*src)(x + s, y + t, c)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                            gy += (*(*src)(x + s, y + t, c)) * m(m.width()/2 + s, m.height()/2 + t, 1);
                        }
                    }
                }
                uint8_t res = (uint8_t)sqrt(gx * gx + gy * gy);
                *((*dst)(x, y, 0)) = res;
                *((*dst)(x, y, 1)) = res;
                *((*dst)(x, y, 2)) = res;
            }
        }
        dst->setName("Фильтр Собеля");
        break;
    }
    case Morphology: //1
    {
        //------------binary img -----------
        ImgData* buf = new ImgData(*src);
        filter(src, buf, Binary);
        //----------------------------------
        Mask m(Morphology);
        // G = A \ (A - B)
        //erosy
        for(int x = m.width()/2; x < buf->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < buf->height() - m.height()/2; y++) {
                bool res = true;

                for(int s = -m.width()/2; s <= m.width()/2; s++) {
                    for(int t = -m.height()/2; t <= m.height()/2; t++) {
                        if(m(m.width()/2 + s, m.height()/2 + t, 0) == 1 && (*(*buf)(x + s, y + t, 0)) == 0) {
                            res = false;
                            break;
                        }
                    }
                }
                for(int c = 0; c < buf->depth(); c++) {
                    *((*dst)(x, y, c)) = res ? 255 : 0;
                }
            }
        }
        // subtract
        for(int x = m.width()/2; x < buf->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < buf->height() - m.height()/2; y++) {
                for(int c = 0; c < buf->depth(); c++) {
                     if((*(*buf)(x, y, c)) != 0 && *((*dst)(x, y, c)) == 0) {
                         *((*dst)(x, y, c)) = 255;
                     } else {
                         *((*dst)(x, y, c)) = 0;
                     }
                }
            }
        }
        delete buf;

        dst->setName("Фильтр Математической морфологии");
        break;
    }
    case Prewitt: //2
    {
        Mask m(Prewitt);

        for(int x = m.width()/2; x < src->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < src->height() - m.height()/2; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int c = 0; c < src->depth(); c++) {
                    for(int s = - m.width()/2; s <= m.width()/2; s++) {
                        for(int t = -m.height()/2; t <= m.height()/2; t++) {
                            gx += (*(*src)(x + s, y + t, c)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                            gy += (*(*src)(x + s, y + t, c)) * m(m.width()/2 + s, m.height()/2 + t, 1);
                        }
                    }
                }
                uint8_t res = (uint8_t)sqrt(gx * gx + gy * gy);
                *((*dst)(x, y, 0)) = res;
                *((*dst)(x, y, 1)) = res;
                *((*dst)(x, y, 2)) = res;
            }
        }
        dst->setName("Фильтр Математической морфологии");
        break;
    }
    case Canny: //3
    {
        dst->setName("Фильтр Математической морфологии");
        break;
    }
    case Laplas: //4
    {

        dst->setName("Фильтр Лапласа");
        break;
    }
    case Roberts: //5
    {
        dst->setName("Фильтр Робертса");
        break;
    }
    case Binary: //6
    {
        uint8_t t = 127;

        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                int val = 0;
                for(int c = 0; c < src->depth(); c++) {
                   val += (*(*src)(x, y, c));
                }
                val/= src->depth();
                for(int c = 0; c < src->depth(); c++) {
                    *((*dst)(x, y, c)) = val >= t ? 255 : 0;
                }
            }
        }
        dst->setName("Бинарный фильтр");
        break;
    }
    case Gauss: //7
    {
        Mask m(Gauss);
        for(int x = m.width()/2; x < src->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < src->height() - m.height()/2; y++) {
                for(int c = 0; c < src->depth(); c++) {
                    double val = 0.0;
                    for(int s = -m.width()/2; s <= m.width()/2; s++) {
                        for(int t = -m.height()/2; t <= m.height()/2; t++) {
                            val += (*(*src)(x + s, y + t, c)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                        }
                    }
                    *((*dst)(x, y, c)) = val;
                }
            }
        }
        dst->setName("Фильтр Гаусса");
        break;
    }
    }
}
