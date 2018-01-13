#include "converter.h"

uint8_t* Converter::RGB2YCbCr(uint8_t* rgb, bool a, bool b, bool c) {
    int     YCbCr[3];
    double  M[3][3] = {{0.299,     0.587,        0.114},
                       {-0.169,    -0.331,       0.500},
                       {0.500,     -0.419,      -0.081}};
    double  RGB[3];

    RGB[0]   = a ? (double)rgb[0] : 0;
    RGB[1]   = b ? (double)rgb[1] : 0;
    RGB[2]   = c ? (double)rgb[2] : 0;

    YCbCr[0] = RGB[0] * M[0][0] + RGB[1] * M[0][1] + RGB[2] * M[0][2];
    YCbCr[1] = RGB[0] * M[1][0] + RGB[1] * M[1][1] + RGB[2] * M[1][2] + 128.0;
    YCbCr[2] = RGB[0] * M[2][0] + RGB[1] * M[2][1] + RGB[2] * M[2][2] + 128.0;

    uint8_t* ycbcr = new uint8_t[3];
    ycbcr[0] = YCbCr[0] > 255 ? 255 : YCbCr[0] < 0 ? 0 : YCbCr[0];
    ycbcr[1] = YCbCr[1] > 255 ? 255 : YCbCr[1] < 0 ? 0 : YCbCr[1];
    ycbcr[2] = YCbCr[2] > 255 ? 255 : YCbCr[2] < 0 ? 0 : YCbCr[2];
    return ycbcr;
}



uint8_t* Converter::YCbCr2RGB(uint8_t* ycbcr, bool a, bool b, bool c) {
    double  M[3][3] = {{1.0,     0.0,        1.402},
                       {1.0,     -0.344,     -0.714},
                       {1.0,     1.722,      0.0}};
    int     RGB[3];
    double  YCbCr[3];

    YCbCr[0] = a ? (double)ycbcr[0] : 0;
    YCbCr[1] = b ? (double)ycbcr[1] - 128.0 : 0;
    YCbCr[2] = c ? (double)ycbcr[2] - 128.0 : 0;

    RGB[0]   = YCbCr[0] * M[0][0] + YCbCr[1] * M[0][1] + YCbCr[2] * M[0][2];
    RGB[1]   = YCbCr[0] * M[1][0] + YCbCr[1] * M[1][1] + YCbCr[2] * M[1][2];
    RGB[2]   = YCbCr[0] * M[2][0] + YCbCr[1] * M[2][1] + YCbCr[2] * M[2][2];

    uint8_t* rgb = new uint8_t[3];
    rgb[0]   = RGB[0] > 255 ? 255 : RGB[0] < 0 ? 0 : RGB[0];
    rgb[1]   = RGB[1] > 255 ? 255 : RGB[1] < 0 ? 0 : RGB[1];
    rgb[2]   = RGB[2] > 255 ? 255 : RGB[2] < 0 ? 0 : RGB[2];
    return rgb;
}

uint8_t* Converter::RGB2HSV(uint8_t* rgb, bool a, bool b, bool c) {
    double MAX = 0, MIN = 0;
    double HSV[3];
    double RGB[3];
    RGB[0] = a ? (double)rgb[0] / 255.0 : 0; // red
    RGB[1] = b ? (double)rgb[1] / 255.0 : 0; // green
    RGB[2] = c ? (double)rgb[2] / 255.0 : 0; // blue

    MAX = max(RGB[0], max(RGB[1], RGB[2]));
    MIN = min(RGB[0], min(RGB[1], RGB[2]));

    if(MAX == MIN)                                  HSV[0] = 0;
    else if(MAX == RGB[0] && RGB[1] >= RGB[2])      HSV[0] = 60.0 * ((RGB[1] - RGB[2]) / (MAX - MIN));
    else if(MAX == RGB[0] && RGB[1] <  RGB[2])      HSV[0] = 60.0 * ((RGB[1] - RGB[2]) / (MAX - MIN)) + 360.0;
    else if(MAX == RGB[1])                          HSV[0] = 60.0 * ((RGB[2] - RGB[0]) / (MAX - MIN)) + 120.0;
    else if(MAX == RGB[2])                          HSV[0] = 60.0 * ((RGB[0] - RGB[1]) / (MAX - MIN)) + 240.0;

    HSV[1] = MAX == 0 ? 0.0 : 1.0 - MIN / MAX;
    HSV[2] = MAX;


    HSV[0] = (HSV[0] * 255.0)/ 360.0;
    HSV[1] = HSV[1] * 255.0;
    HSV[2] = HSV[2] * 255.0;

    uint8_t* hsv  = new uint8_t[3];
    hsv[0]   = HSV[0] > 255 ? 255 : HSV[0] < 0 ? 0 : (int) HSV[0];
    hsv[1]   = HSV[1] > 255 ? 255 : HSV[1] < 0 ? 0 : (int) HSV[1];
    hsv[2]   = HSV[2] > 255 ? 255 : HSV[2] < 0 ? 0 : (int) HSV[2];
    return hsv;
}

//uint8_t* Converter::RGB2HSV(uint8_t* rgb, bool a, bool b, bool c) {
//    double MAX = 0, MIN = 0;
//    double HSV[3];
//    double RGB[3];
//    RGB[0] = a ? (double)rgb[0] / 255.0 : 0; // red
//    RGB[1] = b ? (double)rgb[1] / 255.0 : 0; // green
//    RGB[2] = c ? (double)rgb[2] / 255.0 : 0; // blue

//    MAX = max(RGB[0], max(RGB[1], RGB[2]));
//    MAX = min(RGB[0], min(RGB[1], RGB[2]));

//    if(MAX == MIN)                                  HSV[0] = 0;
//    else if(MAX == RGB[0] && RGB[1] >= RGB[2])      HSV[0] = 60.0 * (RGB[1] - RGB[2]) / (MAX - MIN);
//    else if(MAX == RGB[0] && RGB[1] <  RGB[2])      HSV[0] = 60.0 * (RGB[1] - RGB[2]) / (MAX - MIN) + 360.0;
//    else if(MAX == RGB[1])                          HSV[0] = 60.0 * (RGB[2] - RGB[0]) / (MAX - MIN) + 120.0;
//    else if(MAX == RGB[2])                          HSV[0] = 60.0 * (RGB[0] - RGB[1]) / (MAX - MIN) + 240.0;

//    HSV[1] = MAX == 0 ? 0.0 : 1.0 - MIN / MAX;
//    HSV[2] = MAX;


//    HSV[0] = (HSV[0] * 1.0 / 360.0) * 255.0;
//    HSV[1] = HSV[1] * 255.0;
//    HSV[2] = HSV[2] * 255.0;

//    uint8_t* hsv  = new uint8_t[3];
//    hsv[0]   = HSV[0] > 255 ? 255 : HSV[0] < 0 ? 0 : (int) HSV[0];
//    hsv[1]   = HSV[1] > 255 ? 255 : HSV[1] < 0 ? 0 : (int) HSV[1];
//    hsv[2]   = HSV[2] > 255 ? 255 : HSV[2] < 0 ? 0 : (int) HSV[2];
//    return hsv;
//}

uint8_t* Converter::HSV2RGB(uint8_t* hsv, bool a, bool b, bool c) {
    double  RGB[3];
    double  HSV[3];

    HSV[0] = a ? ((double)hsv[0] * 360.0) / 255.0 : 0;   // hue
    HSV[1] = b ? (double)hsv[1] / 255.0 : 0;           // saturation
    HSV[2] = c ? (double)hsv[2] / 255.0 : 0;           // value

    int Hi = ((int) (HSV[0]) / 60) % 6;

    double f = HSV[0] / 60.0 - ((int) (HSV[0]) / 60);

    double p = HSV[2] * (1.0 - HSV[1]);
    double q = HSV[2] * (1.0 - f * HSV[1]);
    double t = HSV[2] * (1.0 - (1.0 - f) * HSV[1]);

    switch (Hi) {
    case 0:
        RGB[0] = HSV[2];
        RGB[1] = t;
        RGB[2] = p;
        break;
    case 1:
        RGB[0] = q;
        RGB[1] = HSV[2];
        RGB[2] = p;
        break;
    case 2:
        RGB[0] = p;
        RGB[1] = HSV[2];
        RGB[2] = t;
        break;
    case 3:
        RGB[0] = p;
        RGB[1] = q;
        RGB[2] = HSV[2];
        break;
    case 4:
        RGB[0] = t;
        RGB[1] = p;
        RGB[2] = HSV[2];
        break;
    case 5:
        RGB[0] = HSV[2];
        RGB[1] = p;
        RGB[2] = q;
        break;
    }

    RGB[0] *= 255.0;
    RGB[1] *= 255.0;
    RGB[2] *= 255.0;

    uint8_t* rgb = new uint8_t[3];
    rgb[0]   = RGB[0] > 255 ? 255 : RGB[0] < 0 ? 0 : (int) RGB[0];
    rgb[1]   = RGB[1] > 255 ? 255 : RGB[1] < 0 ? 0 : (int) RGB[1];
    rgb[2]   = RGB[2] > 255 ? 255 : RGB[2] < 0 ? 0 : (int) RGB[2];
    return rgb;
}
