#include "transforms.h"

float norm(float min1, float max1, float min2, float max2, float x)
{
    float value = (max2 - min2) * (x - min1) / (max1 - min1) + min2;
    value = value > min2 ? (value > max2 ? max2 : value) : min2;
    return value;
}

void Transforms::onedimHaar(float* data, int len) {
    float* temp = new float[len];
    int h = len / 2;
    for (int i = 0, k = 0; i < h; i++, k = k + 2) {
      temp[i]       = (data[k] + data[k + 1]) / 2;
      temp[i + h]   = (data[k] - data[k + 1]) / 2;
    }
    memcpy(data, temp, len * sizeof(float));
    delete temp;
}
void Transforms::invOnedimHaar(float* data, int len) {
    float* temp = new float[len];

    int h = len / 2;
    for (int i = 0, k = 0; i < h; i++, k = k + 2) {
        temp[k]     = data[i] + data[i + h];
        temp[k + 1] = data[i] - data[i + h];
    }
    memcpy(data, temp, len * sizeof(float));
    delete temp;
}


void Transforms::onedimDobeshi(float* data, int len) {
    float div = 4.0 * sqrt(2.0);
    float d0 = (1.0 + sqrt(3.0)) / div;
    float d1 = (3.0 + sqrt(3.0)) / div;
    float d2 = (3.0 - sqrt(3.0)) / div;
    float d3 = (1.0 - sqrt(3.0)) / div;

    // 0  1  2  3
    // 3 -2  1 -0

    float* temp = new float[len];
    int h = len / 2;
    for (int i = 0; i < h; i++) {
        int k = i * 2;
        if (i < h - 1)
        {
            temp[i]     = data[k] * d0 + data[k + 1] * d1 + data[k + 2] * d2 + data[k + 3] * d3;
            temp[i + h] = data[k] * d3 - data[k + 1] * d2 + data[k + 2] * d1 - data[k + 3] * d0;
        }
        else {
            temp[i]     = data[k] * d0 + data[k + 1] * d1 + data[0] * d2 + data[1] * d3;
            temp[i + h] = data[k] * d3 - data[k + 1] * d2 + data[0] * d1 - data[1] * d0;
        }
        //TODO: last 4 elements1
    }
    memcpy(data, temp, len * sizeof(float));
    delete temp;
}

void Transforms::invOnedimDobeshi(float* data, int len) {
    float div = 4.0 * sqrt(2.0);
    float d0 = (1.0 + sqrt(3.0)) / div;
    float d1 = (3.0 + sqrt(3.0)) / div;
    float d2 = (3.0 - sqrt(3.0)) / div;
    float d3 = (1.0 - sqrt(3.0)) / div;
    float* temp = new float[len];

    int h = len / 2;
    for (int i = 0; i < h; i++) {
        int k = i * 2;
        if (i < h - 1)
        {
            temp[k+2]     = data[i] * d2 + data[i + h] * d1  + data[i + 1] * d0 + data[i + h + 1] * d3;
            temp[k + 1+2] = data[i] * d3 - data[i + h] * d0 + data[i + 1] * d1 - data[i + h + 1] * d2;
        }
        //TODO: last 4 elements
    }
    memcpy(data, temp, len * sizeof(float));
    delete temp;
}

void Transforms::transform(ImgData* src,
                      ImgData* dst,
                      Transform transform,
                      Direction direct,
                      int iterations) {

    Data2d<float>* data = new Data2d<float>(src->width(), src->height(), src->depth());
    for(int x = 0; x < data->width(); x++) {
        for(int y = 0; y < data->height(); y++) {
            for(int c = 0; c < data->depth(); c++) {
                *(*data)(x, y, c) = norm(0, 255, -1, 1, *(*src)(x, y, c));
            }
        }
    }
    float* row = new float[data->width()];
    float* col = new float[data->height()];

    for(int c = 0; c < data->depth(); c++) {
        switch (direct) {
        case Forward:
        {
            for (int k = 1; k <= iterations; k = k * 2) {
                int h = data->height() / k;
                int w = data->width() / k;
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++)
                        row[x] = *(*data)(x, y, c);

                    switch(transform) {
                    case Transform_Haar:
                        onedimHaar(row, w);
                        break;
                    case Transform_Dobeshi:
                        onedimDobeshi(row, w);
                        break;
                    }
                    for (int x = 0; x < w; x++)
                        *(*data)(x, y, c) = row[x];
                }

                for (int x = 0; x < w; x++) {
                    for (int y = 0; y < h; y++)
                        col[y] = *(*data)(x, y, c);

                    switch(transform) {
                    case Transform_Haar:
                        onedimHaar(col, h);
                        break;
                    case Transform_Dobeshi:
                        onedimDobeshi(col, h);
                        break;
                    }
                    for (int y = 0; y < h; y++)
                        *(*data)(x, y, c) = col[y];
                }
            }
            break;
        }
        case Backward:
        {

            for (int k = iterations; k >= 1 ; k = k / 2) {
                int h = data->height() / k;
                int w = data->width() / k;
                for (int x = 0; x < w; x++) {
                    for (int y = 0; y < h; y++)
                        col[y] = *(*data)(x, y, c);

                    switch(transform) {
                    case Transform_Haar:
                        invOnedimHaar(col, h);
                        break;
                    case Transform_Dobeshi:
                        invOnedimDobeshi(col, h);
                        break;
                    }
                    for (int y = 0; y < h; y++)
                        *(*data)(x, y, c) = col[y];
                }
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++)
                        row[x] = *(*data)(x, y, c);
                    switch(transform) {
                    case Transform_Haar:
                        invOnedimHaar(row, w);
                        break;
                    case Transform_Dobeshi:
                        invOnedimDobeshi(row, w);
                        break;
                    }
                    for (int x = 0; x < w; x++)
                        *(*data)(x, y, c) = row[x];
                }
            }
            break;
        }
        }
    }
    delete row;
    delete col;
    for(int x = 0; x < data->width(); x++) {
        for(int y = 0; y < data->height(); y++) {
            for(int c = 0; c < data->depth(); c++) {
                *(*dst)(x, y, c) = norm(-1, 1, 0, 255, *(*data)(x, y, c));
            }
        }
    }
    switch (transform) {
    case Transform_Haar:
        switch (direct) {
        case Forward:
        {
            dst->setName("Haar forward");
            break;
        }
        case Backward:
        {
            dst->setName("Haar backward");
            break;
        }
        }
        break;
    case Transform_Dobeshi:
        switch (direct) {
        case Forward:
        {
            cout<<"Dobeshi forward"<<endl;
            dst->setName("Dobeshi forward");
            break;
        }
        case Backward:
        {
            cout<<"Dobeshi backward"<<endl;
            dst->setName("Dobeshi backward");
            break;
        }
        }
        break;
    }
}
