#include "transforms.h"


void Transforms::onedimHaar(double* data, int len) {
    double* temp = new double[len];
    int h = len / 2;
    for (int i = 0, k = 0; i < h; i++, k = k + 2) {
      temp[i]       = (data[k] + data[k + 1]) / 2;
      temp[i + h]   = (data[k] - data[k + 1]) / 2;
    }
    memcpy(data, temp, len * sizeof(double));
    delete temp;
}
void Transforms::invOnedimHaar(double* data, int len) {
    double* temp = new double[len];

    int h = len / 2;
    for (int i = 0, k = 0; i < h; i++, k = k + 2) {
        temp[k]     = data[i] + data[i + h];
        temp[k + 1] = data[i] - data[i + h];
    }
    memcpy(data, temp, len * sizeof(double));
    delete temp;
}


void Transforms::onedimDobeshi(double* data, int len) {
    double d0 = (1.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d1 = (3.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d2 = (3.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d3 = (1.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));

    double g0 = (1.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g1 = -(3.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g2 = (3.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g3 = -(1.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));

    double* temp = new double[len];
    int h = len / 2;
    for (int i = 0, k = 0; i < h - 1; i++, k = k + 2) {
        if (k < len - 3)
        {
            temp[i] = data[k] * d0 + data[k + 1] * d1 + data[k + 2] * d2 + data[k + 3] * d3;
            temp[i + h] = data[k] * g0 + data[k + 1] * g1 + data[k + 2] * g2 + data[k + 3] * g3;
        }
        //TODO: last 4 elements
    }
    memcpy(data, temp, len * sizeof(double));
    delete temp;
}

void Transforms::invOnedimDobeshi(double* data, int len) {
    double d0 = (1.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d1 = (3.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d2 = (3.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double d3 = (1.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));

    double g0 = (1.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g1 = -(3.0 - sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g2 = (3.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double g3 = -(1.0 + sqrt(3.0)) / (4.0 * sqrt(2.0));
    double id0 = d2;
    double id1 = d1;
    double id2 = d0;
    double id3 = d3;

    double* temp = new double[len];

    int h = len / 2;
    for (int i = 0, k = 0; i < h - 1; i++, k = k + 2) {
        if (k < len - 3)
        {
            temp[k] = data[i] * id0 + data[i + h] * id1 + data[i + 1] * id2 + data[i + h + 1] * id3;
            temp[k + 1] = data[i] * g0 + data[i + h] * g1 + data[i + 1] * g2 + data[i + h + 1] * g3;
        }
        //TODO: last 4 elements
    }
    memcpy(data, temp, len * sizeof(double));
    delete temp;
}

//void Haar(Data2d<double>* data, int iterations, Direction direct) {
//    double* row = new double[data->width()];
//    double* col = new double[data->height()];

//    for(int c = 0; c < data->depth(); c++) {
//        switch (direct) {
//        case Forward:
//        {
//            for (int k = 1; k <= iterations; k = k * 2) {
//                int h = data->height() / k;
//                int w = data->width() / k;
//                for (int y = 0; y < h; y++) {
//                    for (int x = 0; x < w; x++)
//                        row[x] = *(*data)(x, y, c);

//                    onedimHaar(row, w);

//                    for (int x = 0; x < w; x++)
//                        *(*data)(x, y, c) = row[x];
//                }

//                for (int x = 0; x < w; x++) {
//                    for (int y = 0; y < h; y++)
//                        col[y] = *(*data)(x, y, c);

//                    onedimHaar(col, h);
//                    for (int y = 0; y < h; y++)
//                        *(*data)(x, y, c) = col[y];
//                }
//            }
//            break;
//        }
//        case Backward:
//        {

//            for (int k = iterations; k >= 1 ; k = k / 2) {
//                int h = data->height() / k;
//                int w = data->width() / k;
//                for (int x = 0; x < w; x++) {
//                    for (int y = 0; y < h; y++)
//                        col[y] = *(*data)(x, y, c);
//                    invOnedimHaar(col, h);
//                    for (int y = 0; y < h; y++)
//                        *(*data)(x, y, c) = col[y];
//                }
//                for (int y = 0; y < h; y++) {
//                    for (int x = 0; x < w; x++)
//                        row[x] = *(*data)(x, y, c);
//                    invOnedimHaar(row, w);
//                    for (int x = 0; x < w; x++)
//                        *(*data)(x, y, c) = row[x];
//                }
//            }
//            break;
//        }
//        }
//    }
//    delete row;
//    delete col;
//}




void Transforms::transform(ImgData* src,
                      ImgData* dst,
                      Transform transform,
                      Direction direct,
                      int iterations) {

    Data2d<double>* data = new Data2d<double>(src->width(), src->height(), src->depth());
    for(int x = 0; x < data->width(); x++) {
        for(int y = 0; y < data->height(); y++) {
            for(int c = 0; c < data->depth(); c++) {
                *(*data)(x, y, c) = *(*src)(x, y, c);
            }
        }
    }
    double* row = new double[data->width()];
    double* col = new double[data->height()];

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
                *(*dst)(x, y, c) = *(*data)(x, y, c);
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
