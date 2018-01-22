#include "segmentation.h"


float  Segmentation::mean(/*Data2d<uint8_t>* img, */float* h) {
    float m = 0.f;
    int L = 256;
//    for(int x = 0; x < img->width(); x++) {
//        for(int y = 0; y < img->height(); y++) {
//            m += *(*img)(x, y, 0) ;
//        }
//    }
//    m /= img->width() * img->height();
    for(int i = 0; i < L; i++) {
        m += i * h[i];
    }
    return m;
}

void Segmentation::hist(Data2d<uint8_t> *img, float *h) {
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
    int L = 256;

    for(int i = 0; i < L; i++) {
        moment += pow(i - mean, n) * h[i];
    }

}


float Segmentation::R(float variance) {
    return 1.f - 1.f / (1.f + variance);
}

void Segmentation::subset(ImgData* src, Data2d<uint8_t>* sub, int center_x, int center_y) {


    int corner_x0 = 0,
        corner_y0 = 0;

    //left upper corner of subset in src
    int src_corner_x0 = center_x - sub->width() / 2;
    int src_corner_y0 = center_y - sub->height() / 2;

    //right down corner of subset in src
    int src_corner_x1 = center_x + sub->width() / 2;
    int src_corner_y1 = center_y + sub->height() / 2;

    if(src_corner_x0 < 0) {
        corner_x0       = -src_corner_x0;
        src_corner_x0   = 0;
    }
    if(src_corner_y0 < 0) {
        corner_x0       = -src_corner_y0;
        src_corner_y0   = 0;
    }

    if(src_corner_x1 > src->width()) {
        src_corner_x1 = src->width();
    }
    if(src_corner_y1 > src->height()) {
        src_corner_y1 = src->height();
    }


    for(int src_x = src_corner_x0, x = corner_x0; src_x < src_corner_x1; src_x++, x++) {
        for(int src_y = src_corner_y0, y = corner_y0; src_y < src_corner_y1; src_y++, y++) {
            *(*sub)(x, y, 0) = *(*src)(src_x, src_y, 0);
        }
    }
}



void Segmentation::segmentation(ImgData* src,
                                ImgData* dst,
                                Statistic method,
                                int mask_size)
{

    unsigned int nthreads = 1;//std::thread::hardware_concurrency();
    std::cout<<"nthreads: "<<nthreads<<std::endl;

    float**             h     =  new float*[nthreads];
    Data2d<uint8_t>**   sub   = new Data2d<uint8_t>*[nthreads];
    float*              val   = new float[nthreads];

    for(int i = 0; i < nthreads; i++) {
        sub[i] = new Data2d<uint8_t>(mask_size, mask_size, 1);
        h[i] = new float[256];
    }
    Data2d<float>*   buf   = new Data2d<float>(src->width(), src->height(), 1);

    float min = FLT_MAX,
          max = FLT_MIN;
    float m;
    float variance;
    int tid;
    #pragma omp parallel num_threads(nthreads)
    {
#pragma omp for private(variance) private(m) private(tid) schedule(dynamic)
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y ++) {
                tid = omp_get_thread_num();
                subset(src, sub[tid], x, y);
                switch (method) {
                case ThirdMoment:
                {
                    // 1 histogram -> h
                    // 2 mean(h) -> m
                    // 3 moment(h, m, 3) -> val
                    hist(sub[tid], h[tid]);
                    m               = mean(h[tid]);
                    val[tid]        = moment(h[tid], m, 3);
                    break;
                }
                case DesctiptorR:
                {
                    // 1 histogram -> h
                    // 2 mean(h) -> m
                    // 3 moment(h, m, 2) -> variance
                    // R(variance) -> val

                    hist(sub[tid], h[tid]);
                    m               = mean(h[tid]);
                    variance        = moment(h[tid], m, 2);
                    val[tid]        = R(variance);
                    break;
                }
                }
#pragma omp critical
                if(val[tid] > max)
                {
                    max = val[tid];
                }
#pragma omp critical
                if(val[tid] < min) {
                    min = val[tid];
                }
#pragma omp critical
                *(*buf)(x, y, 0) = (uint8_t) val[tid];
        }
    }
    }


    cout<<"max: "<<max<<" min: "<<min<<endl;
    for(int x = 0; x < src->width(); x++) {
        for(int y = 0; y < src->height(); y++) {
            uint8_t res = norm(min, max, 0, 255, *(*buf)(x, y, 0));
            *(*dst)(x, y, 0) = res;
            *(*dst)(x, y, 1) = res;
            *(*dst)(x, y, 2) = res;
        }
    }

    for(int i = 0; i < nthreads; i++) {
        delete sub[i];
        delete h[i];
    }
    delete buf;
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
