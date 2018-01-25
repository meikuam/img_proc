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
    DesctiptorR
};

class Segmentation {

    static void dropRegions(Data2d<int> *src, ImgData* dst, int minSquare = 10);

    static void Fill(Data2d<int> *img, Data2d<int>* labels, int x, int y, int L);
    static void Fill(ImgData* img, Data2d<int>* labels, int x, int y, int L);

    static void labeling(Data2d<int> *img, Data2d<int> *labels);
    static void labeling(ImgData* img, Data2d<int>* labels);

    //----------------------------------------------------------------
    static  float   moment(float* h, float mean, int n);
    static  float   R(float variance);

    static  float   mean(Data2d<uint8_t>* img);
    static  float   mean(float *h);

    static  void    hist(Data2d<uint8_t>* img, float *h);
    static  void    hist(ImgData *img, float* h);

    static  float kmeansThold(Data2d<uint8_t> *img, float eps_min = 5);
    static  float kmeansThold(ImgData *img, float eps_min = 5);

    static  void subset(ImgData* src, Data2d<uint8_t>* sub, int center_x, int center_y);

public:
static void segmentation(ImgData* src, ImgData* dst, Statistic method, int mask_size, bool loc_hist = true, bool multithread = true);
};

#endif // SEGMENTATION_H
