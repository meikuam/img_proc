#include <scrollbar.h>



void ScrollBar::Hide(bool hide) {
    this->setHidden(hide);
}

void ScrollBar::Maximum(int max) {
    this->setMaximum(max);
}
