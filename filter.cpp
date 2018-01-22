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

        ImgData* gray = new ImgData(*src);
        filter(src, gray, Gray);
        ImgData* buf = new ImgData(*src);

        for(int x = m.width()/2; x < src->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < src->height() - m.height()/2; y++) {
                double gx = 0.0,
                       gy = 0.0;
                    for(int s = -m.width()/2; s <= m.width()/2; s++) {
                        for(int t = -m.height()/2; t <= m.height()/2; t++) {
                            gx += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                            gy += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 1);
                        }
                    }
                double res = sqrt(gx * gx + gy * gy);
                res = res > 255 ? 255 : res;
                *((*buf)(x, y, 0)) = (uint8_t)res;
                *((*buf)(x, y, 1)) = (uint8_t)res;
                *((*buf)(x, y, 2)) = (uint8_t)res;
            }
        }
        filter(buf, dst, Binary);
        delete gray;
        delete buf;
        dst->setName(src->getName() + " фильтр Собеля");
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

        dst->setName(src->getName() + " фильтр Математической морфологии");
        break;
    }
    case Prewitt: //2
    {
        Mask m(Prewitt);


        ImgData* gray = new ImgData(*src);
        filter(src, gray, Gray);
        ImgData* buf = new ImgData(*src);

        for(int x = m.width()/2; x < src->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < src->height() - m.height()/2; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int s = - m.width()/2; s <= m.width()/2; s++) {
                    for(int t = -m.height()/2; t <= m.height()/2; t++) {
                        gx += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                        gy += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 1);
                    }
                }
                double res = sqrt(gx * gx + gy * gy);
                res = res > 255 ? 255 : res;
                *((*buf)(x, y, 0)) = (uint8_t)res;
                *((*buf)(x, y, 1)) = (uint8_t)res;
                *((*buf)(x, y, 2)) = (uint8_t)res;
            }
        }
        filter(buf, dst, Binary);
        delete gray;
        delete buf;
        dst->setName(src->getName() + " фильтр Превитта");
        break;
    }
    case Canny: //3
    {
        // Фильтр Гаусса
        ImgData* gauss = new ImgData(*src);
        filter(src, gauss, Gauss);

        // Фильтр Собеля

        ImgData* gray = new ImgData(*src);
        filter(src, gray, Gray);

        Data2d<uint8_t>* sobel = new Data2d<uint8_t>(src->width(), src->height(), 2);
        Mask m(Sobel);
        double maxval = 0.0;
        for(int x = m.width()/2; x < src->width() - m.width()/2; x++) {
            for(int y = m.height()/2; y < src->height() - m.height()/2; y++) {
                double gx = 0.0,
                       gy = 0.0;
                for(int s = -1; s <= 1; s++) {
                    for(int t = -1; t <= 1; t++) {
                        gx += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 0);
                        gy += (*(*gray)(x + s, y + t, 0)) * m(m.width()/2 + s, m.height()/2 + t, 1);
                    }
                }
                double gmod = sqrt(gx * gx + gy * gy);
                gmod = gmod > 255 ? 255 : gmod;
                *((*sobel)(x, y, 0)) = (uint8_t)gmod; // модуль
                if(*((*sobel)(x, y, 0)) > maxval) maxval = *((*sobel)(x, y, 0));

                double angle = abs(atan2(gy, gx)) * 180.0 / 3.14159265;
                if(angle > 0.0 && angle <= 22.5 || angle > 157.5 && angle <= 180.0) {
                    angle = 0.0;
                } else if (angle > 22.5 && angle <= 67.5) {
                    angle = 45.0;
                } else if (angle > 67.5 && angle <= 112.5) {
                    angle = 90.0;
                } else if (angle > 112.5 && angle <= 157.5) {
                    angle = 135.0;
                }
                *((*sobel)(x, y, 1)) = (uint8_t)angle;                   // угол
            }
        }
        delete gauss;

        // Подавление немаксимумов
        Data2d<uint8_t>* nonmax = new Data2d<uint8_t>(src->width(), src->height(), 1);
        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                int left = 0,
                    right = 0;
                int center = (*(*sobel)(x, y, 0));

                switch ((*(*sobel)(x, y, 1))) {
                case 0:
                    if(x > 0 ) left = (*(*sobel)(x - 1, y, 0));
                    if(x < src->width() - 1) right = (*(*sobel)(x + 1, y, 0));
                    break;
                case 45:
                    if(x > 0 && y < src->height() - 1) left = (*(*sobel)(x - 1, y + 1, 0));
                    if(x < src->width() - 1 && y > 0) right = (*(*sobel)(x + 1, y - 1, 0));
                    break;
                case 90:
                    if(y < src->height() - 1) left = (*(*sobel)(x, y + 1, 0));
                    if(y > 0) right = (*(*sobel)(x, y - 1, 0));
                    break;
                case 135:
                    if(x < src->width() - 1 && y < src->height() - 1) left = (*(*sobel)(x + 1, y + 1, 0));
                    if(x > 0 && y > 0) right = (*(*sobel)(x + 1, y + 1, 0));
                    break;
                }
                if(center < left || center < right) {
                    *(*nonmax)(x, y, 0) = 0;
                } else {
                    double res = center * 255.0  / maxval;
                    *(*nonmax)(x, y, 0) = (uint8_t)res;
                }
            }
        }
        delete sobel;

        // Двойная пороговая фильтрация
        uint8_t up_thres    = 80,
                mid_val     = 127,
                down_thres  = 60;

        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                uint8_t val = *(*nonmax)(x, y, 0);
                if(val >= up_thres) {
                    val = 255;
                } else if(val <= down_thres) {
                    val = 0;
                } else {
                    // Трассировка области неоднозначности

                    bool edge = false;

                    if(x > 0 && y >0)                   edge = edge || *(*nonmax)(x - 1, y - 1, 0)  >= up_thres;
                    if(y > 0)                           edge = edge || *(*nonmax)(x, y - 1, 0)      >= up_thres;
                    if(x < src->width() - 1 && y > 0)   edge = edge || *(*nonmax)(x + 1, y - 1, 0)  >= up_thres;
                    if(x > 0)                           edge = edge || *(*nonmax)(x - 1, y, 0)      >= up_thres;
                    if(x < src->width() - 1)            edge = edge || *(*nonmax)(x + 1, y, 0)      >= up_thres;
                    if(x > 0 && y < src->height() - 1)  edge = edge || *(*nonmax)(x - 1, y + 1, 0)  >= up_thres;
                    if(y < src->height() - 1)           edge = edge || *(*nonmax)(x, y + 1, 0)      >= up_thres;
                    if(x < src->width() - 1 &&
                       y < src->height() - 1)           edge = edge || *(*nonmax)(x + 1, y + 1, 0)  >= up_thres;
                    val = edge ? 255 : 0;
                }
                *(*dst)(x, y, 0) = val;
                *(*dst)(x, y, 1) = val;
                *(*dst)(x, y, 2) = val;
            }
        }
        delete nonmax;
        delete gray;
        dst->setName(src->getName() + " фильтр Канни");
        break;
    }
    case Laplas: //4
    {
        dst->setName(src->getName() + " фильтр Лапласа");
        break;
    }
    case Roberts: //5
    {
        dst->setName(src->getName() + " фильтр Робертса");
        break;
    }
    case Binary: //6
    {
        uint8_t t = 127;

        ImgData* gray = new ImgData(*src);
        filter(src, gray, Gray);
        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                for(int c = 0; c < src->depth(); c++) {
                    *((*dst)(x, y, c)) = (*(*gray)(x, y, c)) >= t ? 255 : 0;
                }
            }
        }
        delete gray;
        dst->setName(src->getName() + " бинарный фильтр");
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
        dst->setName(src->getName() + " фильтр Гаусса");
        break;
    }
    case Gray: //8
    {
        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                double g = 0.0;
                for(int c = 0; c < src->depth(); c++) {
                    g += (*(*src)(x, y, c)) * 1.0;
                }
                g /= src->depth();

                for(int c = 0; c < src->depth(); c++) {
                    *((*dst)(x, y, c)) = (uint8_t)g;
                }
            }
        }
        dst->setName(src->getName() + " оттенки серого");
        break;
    }
    case Inversion:
    {
        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                for(int c = 0; c < src->depth(); c++) {
                    *((*dst)(x, y, c)) = *(*src)(x, y, c) > 0 ? 0 : 255;
                }
            }
        }
        dst->setName(src->getName() + " инверсия");
        break;
    }
    }
}
