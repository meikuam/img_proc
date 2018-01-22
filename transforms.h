#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <iostream>
#include <math.h>
#include <float.h>

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

float norm(float from_min, float from_max, float to_min, float to_max, float x);

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
