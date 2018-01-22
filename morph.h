#ifndef MORPH_H
#define MORPH_H

#include <iostream>
#include <math.h>

#include "data2d.h"
#include "imgdata.h"
#include "filter.h"

using namespace std;

class Morph {

public:
    static void morph(ImgData* src,
                      ImgData* dst,
                      ImgData *mask,
                      Method method);
};

#endif // MORPH_H
