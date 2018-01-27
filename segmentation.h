#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <iostream>
#include <math.h>
#include <float.h>
#include <thread>
#include <omp.h>

#include <QSignalSpy>
#include <QTest>

#include "data2d.h"
#include "imgdata.h"
#include "transforms.h"
#include "filter.h"


using namespace std;

enum Statistic {
    ThirdMoment,
    DesctiptorR,
    Uniformity,
    Entropy,
    StandardDeviation,
    Mean
};

class Segmentation {

    static void dropRegions(Data2d<int> *src, ImgData* dst, int regions, int minSquare = 10, bool multithread = true);

    static bool Fill(ImgData* img, Data2d<int>* labels, int x, int y, int L, int val);
    static int labeling(ImgData* img, Data2d<int>* labels, int val);

    //----------------------------------------------------------------

    static  float   mean(ImgData* img);
    static  float   mean(Data2d<uint8_t>* img);
    static  float   mean(float *h);

    static  float   moment(float* h, float mean, int n);
    static  float   R(float variance);
    static  float   U(float* h);
    static  float   entropy(float* h);

    static  void    hist(Data2d<uint8_t>* img, float *h);
    static  void    hist(ImgData *img, float* h);

    static  float kmeansThold(Data2d<uint8_t> *img, float eps_min = 5, bool multithread = true);
    static  float kmeansThold(ImgData *img, float eps_min = 5, bool multithread = true);

    static  void subset(ImgData* src, Data2d<uint8_t>* sub, int center_x, int center_y);

public:

    static void segmentation(ImgData* src_data,
                             ImgData* stat_data, ImgData* bin_data, ImgData* filtred_data, ImgData* out_data,
                             Statistic method,
                             int neighbour, int minSquare,
                             bool useLocalHist = true, bool multithread = true);
};

#endif // SEGMENTATION_H
