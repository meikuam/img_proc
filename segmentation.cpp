#include "segmentation.h"


//int Segmentation::labeling(ImgData* img, Data2d<int>* labels) {
//    int A, B, C;
//    int cur = 1;

//    for (int x = 0; x < img->height(); x++) {
//        for (int y = 0; y < img->width(); y++) {
//            *(*labels)(x, y, 0) = *(*img)(x, y, 0);
//        }
//    }
//    for (int x = 0; x < labels->width(); x++) {
//        for (int y = 0; y < labels->height(); y++) {
//            int kn = y - 1;
//            if (kn < 0) {
//                B = 0;
//            } else {
//                B = *(*labels)(x, kn, 0);
//                int km = x - 1;
//                if (km < 0) {
//                    C = 0;
//                } else {
//                    C = *(*labels)(km, y, 0);
//                    A = *(*labels)(x, y, 0);
//                    if (A == 1){
//                        if (B == 0 && C == 0) {
//                            cur++;
//                            *(*labels)(x, y, 0) = cur;
//                        } else if (B != 0 && C == 0) {
//                            *(*labels)(x, y, 0) = B;
//                        } else if (B == 0 && C != 0) {
//                            *(*labels)(x, y, 0) = C;
//                        } else if (B != 0 && C != 0) {
//                            if (B == C) {
//                                *(*labels)(x, y, 0) = B;
//                            }else {
//                                *(*labels)(x, y, 0) = B;
//                                for (int i = 0; i <= x; i++) {
//                                    for (int j = 0; j < labels->height(); j++) {
//                                        if (i == x && j == y) break;
//                                        if (*(*labels)(i, j) == C)
//                                            *(*labels)(i, j) = B;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return cur;
//}





void Segmentation::dropRegions(Data2d<int>* src, ImgData* dst, int regions, int minSquare, bool multithread) {
//    cout<<"dropRegions:  square calc.";
    unsigned int nthreads = multithread ? std::thread::hardware_concurrency() : 1;

    #pragma omp parallel num_threads(nthreads)
    {
        #pragma omp for schedule(dynamic)
        for(int i = 0; i < regions; i++)
        {
            int square = 0;
            for(int x = 0; x < src->width(); x++) {
                for(int y = 0; y < src->height(); y++) {
                    if(*(*src)(x, y, 0) == i)
                        square++;
                }
            }
            if(square < minSquare) {
//                cout<<"dropRegions:  label("<<i<<") square: "<<square<<" removing; ";
                for(int x = 0; x < src->width(); x++) {
                    for(int y = 0; y < src->height(); y++) {
                        if(*(*src)(x, y, 0) == i)
                            *(*src)(x, y, 0) = 0;
                    }
                }
            } else {
//                cout<<"dropRegions:  label("<<i<<") square: "<<square<<" will remain; ";
            }
        }
    }
//    cout<<"dropRegions:  fill dst image: ";
    for(int x = 0; x < dst->width(); x++) {
        for(int y = 0; y < dst->height(); y++) {
            for(int c = 0; c < dst->depth(); c++)
            *(*dst)(x, y, c) = *(*src)(x, y, 0) > 0 ? 0 : 255;
        }
    }

}

int Segmentation::labeling(ImgData* img, Data2d<int>* labels, int val) {
    int L = 1;
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            *(*labels)(x, y, 0) = 0;
        }
    }
    for(int x = 0; x < img->width(); x++) {
        for(int y = 0; y < img->height(); y++) {
            if(Fill(img, labels, x, y, L, val)) {
                L++;
            }
        }
    }
//    cout<<"labeling:  L: "<<L;
    return L;
}

bool Segmentation::Fill(ImgData* img, Data2d<int>* labels, int x, int y, int L, int val) {
    if((*(*labels)(x, y, 0) == 0) && (*(*img)(x, y, 0) == val)) {
        *(*labels)(x, y, 0) = L;
        if(x > 0)
            Fill(img, labels, x - 1, y, L, val);
        if(x < img->width() - 1)
            Fill(img, labels, x + 1, y, L, val);
        if(y > 0)
            Fill(img, labels, x, y - 1, L, val);
        if(y < img->height() - 1)
            Fill(img, labels, x, y + 1, L, val);
        return true;
    } else {
        return false;
    }
}


void Segmentation::segmentation(ImgData* src_data,
                  ImgData* stat_data, ImgData* bin_data, ImgData* filtred_data, ImgData* out_data,
                  Statistic method,
                  int neighbour, int minSquare,
                  bool useLocalHist, bool multithread)
{
    unsigned int nthreads = multithread ? std::thread::hardware_concurrency() : 1;
    std::cout<<"nthreads: "<<nthreads<<std::endl;

    // vars
    float L = 256;
    int binVal = 0;
    float min = FLT_MAX,
          max = FLT_MIN;

    int width = src_data->width(),
        height = src_data->height();

    float**             h     = new float*[nthreads];
    Data2d<uint8_t>**   sub   = new Data2d<uint8_t>*[nthreads];

    for(int i = 0; i < nthreads; i++) {
        sub[i] = new Data2d<uint8_t>(neighbour, neighbour, 1);
        h[i] = new float[256];
    }

    ImgData*            inp_data        = new ImgData(*src_data);
    Data2d<float>*      stat_data_fp    = new Data2d<float>(width, height, 1);
    // operations

//--------------------------input to gray--------------------------
    cout<<"input to gray"<<endl;
    Filter::filter(inp_data, inp_data, Gray);
//-----------------------gray to stat_data_fp----------------------
    cout<<"gray to stat_data_fp"<<endl;
    if(!useLocalHist)
        hist(inp_data, h[0]);
    #pragma omp parallel for schedule(static) num_threads(nthreads)
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y ++) {
            int tid = omp_get_thread_num();                     // thread id
            float m, variance, val;
            subset(inp_data, sub[tid], x, y);               // get subset of image
            switch (method) {
            case ThirdMoment:
            {
                // 1 histogram -> h
                // 2 mean(h) -> m
                // 3 moment(h, m, 3) -> val
                if(useLocalHist) {
                    hist(sub[tid], h[tid]);
                    m = mean(sub[tid]);
                    val = moment(h[tid], m, 3) / (L * L * L);
                } else {
                    m = mean(sub[tid]);
                    val = moment(h[0], m, 3) / (L * L * L);
                }
                break;
            }
            case DesctiptorR:
            {
                // 1 histogram -> h
                // 2 mean(h) -> m
                // 3 moment(h, m, 2) -> variance
                // R(variance) -> val

                if(useLocalHist) {
                    hist(sub[tid], h[tid]);
                    m = mean(sub[tid]);
                    variance = moment(h[tid], m, 2);
                    val = R(variance);
                } else {
                    m = mean(sub[tid]);
                    variance = moment(h[0], m, 2);
                    val = R(variance);
                }
                break;
            }
            case Uniformity:
            {
                // 1 histogram -> h
                // 2 U -> val
                if(useLocalHist) {
                    hist(sub[tid], h[tid]);
                    val = U(h[tid]);
                }
                break;
            }
            case Entropy:
            {
                // 1 histogram -> h
                // 2 entropy
                if(useLocalHist) {
                    hist(sub[tid], h[tid]);
                    val = entropy(h[tid]);
                }
                break;
            }
            case StandardDeviation:
            {

                if(useLocalHist) {
                    hist(sub[tid], h[tid]);
                    m = mean(sub[tid]);
                    variance = moment(h[tid], m, 2);
                    val = sqrt(variance);
                } else {
                    m = mean(sub[tid]);
                    variance = moment(h[0], m, 2);
                    val = sqrt(variance);
                }
                break;
            }
            case Mean:
            {
                val = mean(sub[tid]);
                break;
            }
            }
            //                    cout<<val<<" ";
            //                    val = val >  250 ?  250 : val;
            //                    val = val < -250 ? -250 : val;
            #pragma omp critical
            {
                if(val > max)
                {
                    max = val;
                }
                if(val < min) {
                    min = val;
                }
                *(*stat_data_fp)(x, y, 0) = val;
            }
            }
        }
    cout<<"max: "<<max<<" min: "<<min<<endl;
//--------------stat_data_fp -> normalize -> stat_data-------------
    cout<<"stat_data_fp -> normalize -> stat_data"<<endl;
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            uint8_t res = norm(min, max, 0, 255, *(*stat_data_fp)(x, y, 0));

            for(int c = 0; c < stat_data->depth(); c++) {
                *(*stat_data)(x, y, c) = res;
            }
        }
    }

//--------------stat_data -> threshold -> bin_data-----------------
    //TODO: ручное задание порога бинаризации и другие методы
    cout<<"stat_data -> threshold -> bin_data"<<endl;
    hist(stat_data, h[0]);
    float T = kmeansThold(stat_data, 10, multithread);                   // set threshold

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            for(int c = 0; c < stat_data->depth(); c++) {
                *(*bin_data)(x, y, c) = *(*stat_data)(x, y, c) > T ? 255 : 0;
            }
        }
    }
//-------------bin_data -> (minSquare) -> filtred_data-------------
    cout<<"bin_data -> (minSquare) -> filtred_data"<<endl;
    //feature
    switch (method) {
    case ThirdMoment:
        binVal = 0;
        break;
    case DesctiptorR:
        if(useLocalHist)
            binVal = 0;
        else
            binVal = 255;
        break;
    case Uniformity:
        binVal = 255;
        break;
    case Entropy:
        binVal = 255;
        break;
    case StandardDeviation:
        if(useLocalHist)
            binVal = 0;
        else
            binVal = 255;
        break;
    case Mean:
        binVal = 0;
        break;
    }

    cout<<" labeling(bin_data, labels, 0);"<<endl;
    Data2d<int>* labels = new Data2d<int>(width, height, 1);
    int l = labeling(bin_data, labels, binVal);
//    *filtred_data = *bin_data;
    cout<<" dropRegions(labels, filtred_data)"<<endl;
    dropRegions(labels, filtred_data, l, minSquare, multithread);
//-----filtred_data -> Morphology Edge Detection -> edge_data------
    ImgData* edge_data = new ImgData(*filtred_data);
    Filter::filter(filtred_data, edge_data, Morphology);
//--------------filtred_data -> (src_data) -> out_data-------------
    cout<<"filtred_data -> (src_data) -> out_data"<<endl;
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            if(*(*filtred_data)(x, y, 0) == 255) {
                for(int c = 0; c < out_data->depth(); c++) {
                    *(*out_data)(x, y, c) = *(*src_data)(x, y, c);
                }
            }
            if(*(*edge_data)(x, y, 0) == 255) {
                *(*out_data)(x, y, 0) = 250;
                *(*out_data)(x, y, 1) = 20;
                *(*out_data)(x, y, 2) = 20;
            }
            if(*(*filtred_data)(x, y, 0) == 0 && *(*edge_data)(x, y, 0) == 0) {
                for(int c = 0; c < out_data->depth(); c++) {
                    *(*out_data)(x, y, c) = 0;
                }
            }
        }
    }

    delete stat_data_fp;
    delete inp_data;
    for(int i = 0; i < nthreads; i++) {
        delete sub[i];
        delete h[i];
    }
    delete sub;
    delete h;
    delete labels;
}

//----------------------------------------------------------------

float  Segmentation::mean(ImgData* img) {
    int w = img->width(),
        h = img->height();
    float m = 0.f,
          n = w * h;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            m += *(*img)(x, y, 0);
        }
    }
    m /= n;
    return m;
}

float  Segmentation::mean(Data2d<uint8_t>* img) {
    int w = img->width(),
        h = img->height();
    float m = 0.f,
          n = w * h;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            m += *(*img)(x, y, 0);
        }
    }
    m /= n;
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
    int L = 256;

    for(int i = 0; i < L; i++) {
        moment += pow(i - mean, n) * h[i];
    }
    return moment;
}


float Segmentation::R(float variance) {
    float L2 = 256 * 256;
    return 1.f - (1.f / (1.f + variance / L2));
}

float Segmentation::U(float* h) {
    float u = 0.f;
    int L = 256;

    for(int i = 0; i < L; i++) {
        u += h[i] * h[i];
    }
    return u;
}

float Segmentation::entropy(float* h) {
    double e = 0.f;
    int L = 256;

    for(int i = 0; i < L; i++) {
        if(h[0] > 0) {
            e += h[i] * log2(h[i]);
        }
    }
    return - e;
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
    int src_x1_ = src_x1;
    int src_y1_ = src_y1;
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

    if(src_x0 < 0) {
        x0       = -src_x0;
        src_x0   = 0;
    }
    if(src_y0 < 0) {
        y0       = -src_y0;
        src_y0   = 0;
    }

    if(src_x1 > src->width()) {
        src_x1 = src->width();
    }
    if(src_y1 > src->height()) {
        src_y1 = src->height();
    }

    if(x0 != 0 || y0 != 0) {
        for(int src_x = src_x0, x = 0; x < x0; src_x++, x++) {
            for(int src_y = src_y0, y = 0; y < y0; src_y++, y++) {
                *(*sub)(x, y, 0) = *(*src)(src_x, src_y, 0);
            }
        }
    }
// TODO: corners
//    if(src_x1 != src_x1_ || src_y1 != src_y1_) {
//        for(int src_x = src_x1, x = 0; x < sub->width(); src_x++, x++) {
//            for(int src_y = src_y1, y = 0; y < sub->height(); src_y++, y++) {
//                *(*sub)(x, y, 0) = *(*src)(src_x, src_y, 0);
//            }
//        }
//    }
    for(int src_x = src_x0, x = x0; src_x < src_x1; src_x++, x++) {
        for(int src_y = src_y0, y = y0; src_y < src_y1; src_y++, y++) {
            *(*sub)(x, y, 0) = *(*src)(src_x, src_y, 0);
        }
    }
}

//----------------------------------------------------------------
float Segmentation::kmeansThold(Data2d<uint8_t>* img, float eps_min, bool multithread) {
    float eps = 256.f;
    float T = 127.f;
    float m1, m2;

    while(eps > eps_min) {
        if(multithread) {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                m1 = 0.f;
                for(int x = 0; x < img->width(); x++) {
                    for(int y = 0; y < img->height(); y++) {
                        if(*(*img)(x, y, 0) < T) {
                            m1 += *(*img)(x, y, 0);
                        }
                    }
                }
                m1 /= img->width() * img->height();
            }
            #pragma omp section
            {
                m2 = 0.f;
                for(int x = 0; x < img->width(); x++) {
                    for(int y = 0; y < img->height(); y++) {
                        if(*(*img)(x, y, 0) >= T) {
                            m2 += *(*img)(x, y, 0);
                        }
                    }
                }
                m2 /= img->width() * img->height();
            }
        }
        } else {
            m1 = 0.f;
            m2 = 0.f;
            for(int x = 0; x < img->width(); x++) {
                for(int y = 0; y < img->height(); y++) {
                    if(*(*img)(x, y, 0) < T) {
                        m1 += *(*img)(x, y, 0);
                    }
                }
            }
            for(int x = 0; x < img->width(); x++) {
                for(int y = 0; y < img->height(); y++) {
                    if(*(*img)(x, y, 0) >= T) {
                        m2 += *(*img)(x, y, 0);
                    }
                }
            }
            m1 /= img->width() * img->height();
            m2 /= img->width() * img->height();
        }
        float bufT = (m1 + m2) / 2.f;
        bufT = bufT > 255 ? 255 : bufT < 0 ? 0 : bufT;
        eps = abs(T - bufT);
        cout<<"bufT: "<<bufT<<endl;
        T = bufT;
    }
    cout<<"T: "<<T<<endl;
    return T;
}


float Segmentation::kmeansThold(ImgData *img, float eps_min, bool multithread) {
    float eps = 256.f;
    float T = 127.f;
    float m1, m2;

    while(eps > eps_min) {
        if(multithread) {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                m1 = 0.f;
                for(int x = 0; x < img->width(); x++) {
                    for(int y = 0; y < img->height(); y++) {
                        if(*(*img)(x, y, 0) < T) {
                            m1 += *(*img)(x, y, 0);
                        }
                    }
                }
                m1 /= img->width() * img->height();
            }
            #pragma omp section
            {
                m2 = 0.f;
                for(int x = 0; x < img->width(); x++) {
                    for(int y = 0; y < img->height(); y++) {
                        if(*(*img)(x, y, 0) >= T) {
                            m2 += *(*img)(x, y, 0);
                        }
                    }
                }
                m2 /= img->width() * img->height();
            }
        }
        } else {
            m1 = 0.f;
            m2 = 0.f;
            for(int x = 0; x < img->width(); x++) {
                for(int y = 0; y < img->height(); y++) {
                    if(*(*img)(x, y, 0) < T) {
                        m1 += *(*img)(x, y, 0);
                    }
                }
            }
            for(int x = 0; x < img->width(); x++) {
                for(int y = 0; y < img->height(); y++) {
                    if(*(*img)(x, y, 0) >= T) {
                        m2 += *(*img)(x, y, 0);
                    }
                }
            }
            m1 /= img->width() * img->height();
            m2 /= img->width() * img->height();
        }
        float bufT = (m1 + m2) / 2.f;
        bufT = bufT > 255 ? 255 : bufT < 0 ? 0 : bufT;
        eps = abs(T - bufT);
        cout<<"bufT: "<<bufT<<endl;
        T = bufT;
    }
    cout<<"T: "<<T<<endl;
    return T;
}
