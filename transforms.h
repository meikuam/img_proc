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
    static void onedimHaar(float* data, int len);
    static void invOnedimHaar(float* data, int len);
    static void onedimDobeshi(float* data, int len);
    static void invOnedimDobeshi(float* data, int len);

public:
    static void transform(ImgData* src,
                          ImgData* dst,
                          Transform transform,
                          Direction direction,
                          int iterations);
};

#endif // TRANSFORMS_H
