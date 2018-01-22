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

using namespace std;

enum Statistic {
    ThirdMoment,
    DesctiptorR
};

class Segmentation {
    static  void    hist(Data2d<uint8_t>* img, float *h);
    static  float   mean(float *h);
    static  float   moment(float* h, float mean, int n);
    static  float   R(float variance);

    static void subset(ImgData* src, Data2d<uint8_t>* sub, int center_x, int center_y);
public:
static void segmentation(ImgData* src, ImgData* dst, Statistic method, int mask_size);
};

#endif // SEGMENTATION_H
