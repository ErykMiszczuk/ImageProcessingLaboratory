#include "morph_dilate.h"

MorphDilate::MorphDilate(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphDilate::MorphDilate(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphDilate::morph(math::matrix<float> window, math::matrix<bool> se)
{
    float min = PIXEL_VAL_MAX+1;

   // qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x=0; x < window.colno(); x++)
        for (int y=0; y<window.rowno();y++)
            if (se(x,y) && window (x,y) <min) min = window (x,y);

    return min;
}
