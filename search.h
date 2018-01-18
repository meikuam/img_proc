#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <math.h>

#include "data2d.h"
#include "imgdata.h"

using namespace std;

class Search {

public:
    static void search(ImgData* src,
                          ImgData* dst);
};

#endif // SEARCH_H
