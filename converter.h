#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

using namespace std;

class Converter {
public:
    static void RGB2YCbCr(uint8_t* rgb,
                          uint8_t* ycbcr,
                          bool a = true, bool b = true, bool c = true);
    static void RGB2HSV(uint8_t* rgb,
                        uint8_t* hsv,
                        bool a = true, bool b = true, bool c = true);
    static void YCbCr2RGB(uint8_t* ycbcr,
                          uint8_t* rgb,
                          bool a = true, bool b = true, bool c = true);
    static void HSV2RGB(uint8_t* hsv,
                        uint8_t* rgb,
                        bool a = true, bool b = true, bool c = true);
};

#endif // CONVERTER_H
