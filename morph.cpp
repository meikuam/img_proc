#include "morph.h"


void Morph::morph(ImgData *src,
                  ImgData *dst,
                  ImgData *mask,
                  Method method) {
    switch (method) {
    case Erosion:
    {
        for(int x = mask->width()/2; x < src->width() - mask->width()/2; x++) {
            for(int y = mask->height()/2; y < src->height() - mask->height()/2; y++) {
                for(int c = 0; c < src->depth(); c++) {

                    bool check = true;
                    for(int s = -mask->width()/2; s <= mask->width()/2; s++) {
                        for(int t = -mask->height()/2; t <= mask->height()/2; t++) {
                            if(*(*mask)(mask->width()/2 + s, mask->height()/2 + t, c) == 255) {
                                if(*(*src)(x + s, y + t, c) != *(*mask)(mask->width()/2 + s, mask->height()/2 + t, c)){
                                    check = false;
                                }
                            }
                        }
                    }
                    *((*dst)(x, y, 0)) = check ? 255 : 0;
                    *((*dst)(x, y, 1)) = check ? 255 : 0;
                    *((*dst)(x, y, 2)) = check ? 255 : 0;
                }
            }
        }
        dst->setName("("+src->getName() + ") эрозия по (" + mask->getName()+")");
        break;
    }
    case Dilation:
    {
        for(int x = mask->width()/2; x < src->width() - mask->width()/2; x++) {
            for(int y = mask->height()/2; y < src->height() - mask->height()/2; y++) {
                for(int c = 0; c < src->depth(); c++) {

                    bool check = false;
                    for(int s = -mask->width()/2; s <= mask->width()/2; s++) {
                        for(int t = -mask->height()/2; t <= mask->height()/2; t++) {
                            if(*(*mask)(mask->width()/2 + s, mask->height()/2 + t, c) == 255) {
                                if(*(*src)(x + s, y + t, c) == *(*mask)(mask->width()/2 + s, mask->height()/2 + t, c)){
                                    check = true;
                                }
                            }
                        }
                    }
                    *((*dst)(x, y, c)) = check ? 255 : 0;
                }
            }
        }
        dst->setName("("+src->getName() + ") дилатация по (" + mask->getName() + ")");
        break;
    }
    case Subtraction:
    {
        if(mask->width() == src->width() &&
                mask->height() == src->height())
        {
            for(int x = 0; x < src->width(); x++) {
                for(int y = 0; y < src->height(); y++) {
                    for(int c = 0; c < src->depth(); c++) {
                        *((*dst)(x, y, c)) = *(*src)(x, y, c) > 0 && *(*mask)(x, y, c) == 0 ? 255 : 0;
                    }
                }
            }
        }
        dst->setName("("+src->getName() + ") вычитание по (" + mask->getName() + ")");
        break;
    }
    case Union:
    {
        if(mask->width() == src->width() &&
                mask->height() == src->height())
        {
            for(int x = 0; x < src->width(); x++) {
                for(int y = 0; y < src->height(); y++) {
                    for(int c = 0; c < src->depth(); c++) {
                        *((*dst)(x, y, c)) = *(*src)(x, y, c) > 0 || *(*mask)(x, y, c) > 0 ? 255 : 0;
                    }
                }
            }
        }
        dst->setName("("+src->getName() + ") объединение по (" + mask->getName() + ")");
        break;
    }
    case Intersection:
    {
        if(mask->width() == src->width() &&
                mask->height() == src->height())
        {
            for(int x = 0; x < src->width(); x++) {
                for(int y = 0; y < src->height(); y++) {
                    for(int c = 0; c < src->depth(); c++) {
                        *((*dst)(x, y, c)) = *(*src)(x, y, c) > 0 && *(*mask)(x, y, c) > 0 ? 255 : 0;
                    }
                }
            }
        }
        dst->setName("("+src->getName() + ") пересечение по (" + mask->getName() + ")");
        break;
    }
    }
}
