#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

using namespace std;

class Converter {
public:
    static uint8_t* RGB2YCbCr(uint8_t* rgb);
    static uint8_t* RGB2HSV(uint8_t* rgb);
    static uint8_t* YCbCr2RGB(uint8_t* ycbcr);
    static uint8_t* HSV2RGB(uint8_t* hsv);
};

#endif // CONVERTER_H
