#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <iostream>
#include <math.h>

#include "data2d.h"
#include "imgdata.h"

using namespace std;

enum Transform{
    Transform_Haar,
    Transform_Dobeshi
};
enum Direction {
    Forward,
    Backward
};

class Transforms {
    static void onedimHaar(double* data, int len);
    static void invOnedimHaar(double* data, int len);
    static void onedimDobeshi(double* data, int len);
    static void invOnedimDobeshi(double* data, int len);
    static void Haar(Data2d<double>* data, int iterations, Direction direct);
    static void Dobeshi(Data2d<double>* data, int iterations, Direction direct);

public:
    static void transform(ImgData* src,
                          ImgData* dst,
                          Transform transform,
                          Direction direction,
                          int iterations);
};

#endif // TRANSFORMS_H
