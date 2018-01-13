#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

using namespace std;

class Converter {
public:
    static uint8_t* RGB2YCbCr(uint8_t* rgb, bool a = true, bool b = true, bool c = true);
    static uint8_t* RGB2HSV(uint8_t* rgb, bool a = true, bool b = true, bool c = true);
    static uint8_t* YCbCr2RGB(uint8_t* ycbcr, bool a = true, bool b = true, bool c = true);
    static uint8_t* HSV2RGB(uint8_t* hsv, bool a = true, bool b = true, bool c = true);
};

#endif // CONVERTER_H
