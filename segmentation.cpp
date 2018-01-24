#include "segmentation.h"


//http://www.lib.tpu.ru/fulltext/c/2010/C04/V1/C04_V1.pdf

float  Segmentation::mean(Data2d<uint8_t>* img/*, float* h*/) {
    float m = 0.f;
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            m += *(*img)(x, y, 0);
        }
    }
    m /= img->width() * img->height();
    return m;
}

float  Segmentation::mean(float* h) {
    float m = 0.f;
    int L = 256;
    for(int i = 0; i < L; i++) {
        m += i * h[i];
    }
    m * L;
    return m;
}

void Segmentation::hist(Data2d<uint8_t> *img, float* h) {
    float n = img->width() * img->height();
    int L = 256;
    for(int i = 0; i < L; i++) {
        h[i] = 0.f;
    }
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            int num = *(*img)(x, y, 0);
            h[num] += 1;
        }
    }
    for(int i = 0; i < L; i++) {
        h[i] /= n;
    }
}

void Segmentation::hist(ImgData *img, float* h) {
    float n = img->width() * img->height();
    int L = 256;
    for(int i = 0; i < L; i++) {
        h[i] = 0.f;
    }
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            int num = *(*img)(x, y, 0);
            h[num] += 1;
        }
    }
    for(int i = 0; i < L; i++) {
        h[i] /= n;
    }
}

float Segmentation::moment(float* h, float mean, int n) {
    if(n == 0) return 1.f;
    if(n == 1) return 0.f;
    float moment = 0.f;
//    float value = 0.f;
    int L = 256;

    for(int i = 0; i < L; i++) {
        moment += pow(i - mean, n)* h[i];
    }
    return moment;
}


float Segmentation::R(float variance) {
    return 1.f - 1.f / (1.f + variance);
}

void Segmentation::subset(ImgData* src, Data2d<uint8_t>* sub, int center_x, int center_y) {

    for(int x = 0; x < sub->width(); x++) {
        for(int y = 0; y < sub->height(); y++) {
            *(*sub)(x, y, 0) = 0;
        }
    }
    int x0 = 0,
        y0 = 0;

    //left upper corner of subset in src
    int src_x0 = center_x - sub->width() / 2;
    int src_y0 = center_y - sub->height() / 2;

    //right down corner of subset in src
    int src_x1 = src_x0 + sub->width();
    int src_y1 = src_y0 + sub->height();

    if(src_x0 < 0) {
        x0       = -src_x0;
        src_x0   = 0;
    }
    if(src_y0 < 0) {
        y0       = -src_y0;
        src_y0   = 0;
    }
//    _____________________
//    |    |              |
//    |    |              |
//    |    |              |
//    ---------------------
//    |    |corner of     |
//    |    |image         |
//    |    |              |
//    |    |              |
//    ---------------------

    if(src_x1 > src->width()) {
        src_x1 = src->width();
    }
    if(src_y1 > src->height()) {
        src_y1 = src->height();
    }
    for(int src_x = src_x0, x = x0; src_x < src_x1; src_x++, x++) {
        for(int src_y = src_y0, y = y0; src_y < src_y1; src_y++, y++) {
            *(*sub)(x, y, 0) = *(*src)(src_x, src_y, 0);
        }
    }
}

float Segmentation::kmeansThold(Data2d<uint8_t>* img, float eps_max) {
    float eps = 256.f;
    float T = 127.f;
    float m1 = 0.f;
    float m2 = 0.f;
    while(eps > eps_max) {
//#pragma omp parallel sections
//{
//#pragma omp section
//    {
        m1 = 0.f;
        for(int x = 0; x < img->width(); x++) {
            for(int y = 0; y < img->height(); y++) {
                if(*(*img)(x, y, 0) < T) {
                    m1 += *(*img)(x, y, 0);
                }
            }
        }
//        for(int i = 0; i < T; i++) {
//            m1 += hist[i];
//        }
//    }
//#pragma omp section
//    {

        m2 = 0.f;
        for(int x = 0; x < img->width(); x++) {
            for(int y = 0; y < img->height(); y++) {
                if(*(*img)(x, y, 0) >= T) {
                    m2 += *(*img)(x, y, 0);
                }
            }
        }
//        for(int i = T; i < 256; i++) {
//            m2 += hist[i];
//        }
//    }
//}

        m1 /= img->width() * img->height();
        m2 /= img->width() * img->height();
        float bufT = (m1 + m2) / 2.f;
        eps = abs(T - bufT);
        T = bufT;
    }
    cout<<"T: "<<T<<endl;
    return T;
}

void Segmentation::dropRegions(Data2d<int>* src, ImgData* dst, int minSquare) {
//ищем черные области
    int max = 0;
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y++) {
            if(*(*src)(x, y, 0) > max)
                max = *(*src)(x, y, 0);
        }
    }
    for(int i = 0; i < max; i++)
    {
        int reg = 0;
        for(int x = 0; x < src->width(); x++) {
            for(int y = 0; y < src->height(); y++) {
                if(*(*src)(x, y, 0) == i)
                    reg++;
            }
        }
        if(reg < minSquare) {
            for(int x = 0; x < src->width(); x++) {
                for(int y = 0; y < src->height(); y++) {
                    if(*(*src)(x, y, 0) == i)
                        *(*src)(x, y, 0) = 0;
                }
            }
        }
    }
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y++) {
            *(*dst)(x, y, 0) = *(*src)(x, y, 0) > 0 ? 255 : 0;
        }
    }

}

void Segmentation::labeling(ImgData* img, Data2d<int>* labels) {
    int L = 1;
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            Fill(img, labels, x, y, L++);
        }
    }
}

void Segmentation::Fill(ImgData* img, Data2d<int>* labels, int x, int y, int L) {
    if((*(*labels)(x,y,0) == 0) && (*(*img)(x,y,0) == 0)) {
        *(*labels)(x,y,0) = L;
        if(x > 0)
            Fill(img, labels, x - 1, y, L);
        if(x < img->width() - 1)
            Fill(img, labels, x + 1, y, L);
        if(y > 0)
            Fill(img, labels, x, y - 1, L);
        if(y < img->height() - 1)
            Fill(img, labels, x, y + 1, L);
    }
}

void Segmentation::labeling(Data2d<int> *img, Data2d<int>* labels) {
    int L = 1;
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            Fill(img, labels, x, y, L++);
        }
    }
}

void Segmentation::Fill(Data2d<int>* img, Data2d<int>* labels, int x, int y, int L) {
    if((*(*labels)(x,y,0) == 0) && (*(*img)(x,y,0) == 0)) {
        *(*labels)(x,y,0) = L;
        if(x > 0)
            Fill(img, labels, x - 1, y, L);
        if(x < img->width() - 1)
            Fill(img, labels, x + 1, y, L);
        if(y > 0)
            Fill(img, labels, x, y - 1, L);
        if(y < img->height() - 1)
            Fill(img, labels, x, y + 1, L);
    }
}

void Segmentation::segmentation(ImgData* src,
                                ImgData* dst,
                                Statistic method,
                                int mask_size)
{

    unsigned int nthreads  = 1;//= std::thread::hardware_concurrency();
    std::cout<<"nthreads: "<<nthreads<<std::endl;
float* h = new float[256];
//    float**             h     = new float*[nthreads];
    Data2d<uint8_t>**   sub   = new Data2d<uint8_t>*[nthreads];
    float              val;//   = new float[nthreads];

    for(int i = 0; i < nthreads; i++) {
        sub[i] = new Data2d<uint8_t>(mask_size, mask_size, 1);
//        h[i] = new float[256];
    }
    Data2d<float>*   buf   = new Data2d<float>(src->width(), src->height(), 1);
    Data2d<uint8_t>* res_buf   = new Data2d<uint8_t>(src->width(), src->height(), 1);
    ImgData*  input = new ImgData(*src);
    Filter::filter(input, input, Gray);

    float min = FLT_MAX,
          max = FLT_MIN;
    float m;
    float variance;
    int tid;
//    hist(src, h);
    #pragma omp parallel num_threads(nthreads)
    {
#pragma omp for private(variance) private(m) private(tid) private(val) schedule(dynamic)
    for(int x = mask_size/2 ; x < src->width() - mask_size/2; x++) {
        for(int y =mask_size/2 ; y < src->height() - mask_size/2 ; y ++) {
                tid = omp_get_thread_num();
                subset(input, sub[tid], x, y);
                switch (method) {
                case ThirdMoment:
                {
                    // 1 histogram -> h
                    // 2 mean(h) -> m
                    // 3 moment(h, m, 3) -> val
                    hist(sub[tid], h);
                    m               = /*mean(sub[tid]); //*/mean(h);
                    //                    val             = moment(h[tid], m, 3);
                    val             = moment(h, m, 3);
                    break;
                }
                case DesctiptorR:
                {
                    // 1 histogram -> h
                    // 2 mean(h) -> m
                    // 3 moment(h, m, 2) -> variance
                    // R(variance) -> val

                    hist(sub[tid], h);
                    m               = /*mean(sub[tid]); //*/mean(h);
//                    variance        = moment(h[tid], m, 2);
                    variance        = moment(h, m, 2);
                    val             = R(variance);
                    break;
                }
                }

#pragma omp critical
                if(val > 1270) {
                    val = 1270;
                }
#pragma omp critical
                if(val < -1270)
                {
                    val = -1270;
                }
#pragma omp critical
                if(val > max)
                {
                    max = val;
                }
#pragma omp critical
                if(val < min) {
                    min = val;
                }
#pragma omp critical
                *(*buf)(x, y, 0) = val;
        }
    }
    }
    cout<<"max: "<<max<<" min: "<<min<<endl;
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y++) {
            uint8_t res = norm(min, max, 0, 255, *(*buf)(x, y, 0));
            *(*res_buf)(x, y, 0) = res;
        }
    }
    float* h_img = new float[256];
    hist(res_buf, h_img);

    float T = kmeansThold(res_buf, 5);

    Data2d<int>* kek_img = new Data2d<int>(src->width(), src->height(), 1);
    Data2d<int>* kek_lab = new Data2d<int>(src->width(), src->height(), 1);
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y++) {
            *(*kek_img)(x, y, 0) = *(*res_buf)(x, y, 0) > T ? 255 : 0;
            *(*kek_lab)(x, y, 0) = 0;
        }
    }
//    for(int x = 0; x < src->width(); x++) {
//        for(int y = 0; y < src->height(); y++) {
//            *(*res_buf)(x, y, 0) = *(*res_buf)(x, y, 0) > T ? 255 : 0;
//        }
//    }

    //----------------


    labeling(kek_img, kek_lab);
    dropRegions(kek_lab, dst, 50);
    //----------------
//    for(int x = 0; x < src->width(); x++) {
//        for(int y = 0; y < src->height(); y++) {
//            uint8_t res = *(*res_buf)(x, y, 0);

//            *(*dst)(x, y, 0) = *(*res_buf)(x, y, 0);
//            *(*dst)(x, y, 1) = *(*res_buf)(x, y, 0) > T ? 255 : 0;;
////            *(*dst)(x, y, 2) = *(*res_buf)(x, y, 0);

////            if(res > 0) {
////                *(*dst)(x, y, 0) = *(*src)(x, y, 0);
////                *(*dst)(x, y, 1) = *(*src)(x, y, 1);
////                *(*dst)(x, y, 2) = *(*src)(x, y, 2);
////            } else {
////                *(*dst)(x, y, 0) = 0;
////                *(*dst)(x, y, 1) = 20;
////                *(*dst)(x, y, 2) = 255;
////            }
//        }
//    }

    for(int i = 0; i < nthreads; i++) {
        delete sub[i];
//        delete h[i];
    }
    delete input;
    delete buf;
    delete res_buf;
    delete sub;
    delete h;
}

//void Segmentation::segmentation(ImgData* src,
//                                ImgData* dst,
//                                Statistic method,
//                                int mask_size)
//{

//    float* h = new float[256];
//    float val;
//    float min = FLT_MAX,
//          max = FLT_MIN;

//    Data2d<uint8_t>* sub = new Data2d<uint8_t>(mask_size, mask_size, 1);
//    Data2d<float>*   buf   = new Data2d<float>(src->width(), src->height(), 1);

//    for(int x = 0; x < src->width(); x++) {
//        for(int y = 0; y < src->height(); y++) {
//            subset(src, sub, x, y);
//            switch (method) {
//            case ThirdMoment:
//            {
//                // 1 histogram -> h
//                // 2 mean(h) -> m
//                // 3 moment(h, m, 3) -> val
//                hist(sub, h);
//                float m = mean(h);
//                val     = moment(h, m, 3);
//                break;
//            }
//            case DesctiptorR:
//            {
//                // 1 histogram -> h
//                // 2 mean(h) -> m
//                // 3 moment(h, m, 2) -> variance
//                // R(variance) -> val

//                hist(sub, h);
//                float m         = mean(h);
//                float variance  = moment(h, m, 2);
//                val             = R(variance);
//                break;
//            }
//            }
//            if(val > max) max = val;
//            if(val < min) min = val;
//            *(*buf)(x, y, 0) = val;
//        }
//    }

//    for(int x = 0; x < src->width(); x++) {
//        for(int y = 0; y < src->height(); y++) {
//           *(*dst)(x, y, 0) = *(*dst)(x, y, 1) = *(*dst)(x, y, 2) = norm(min, max, 0, 255, *(*buf)(x, y, 0));
//        }
//    }
//    delete buf;
//    delete sub;
//    delete h;
//}
