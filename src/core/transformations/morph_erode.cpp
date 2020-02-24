#include "morph_erode.h"

MorphErode::MorphErode(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphErode::MorphErode(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphErode::morph(math::matrix<float> window, math::matrix<bool> se)
{
    float max=0.0;

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x=0; x < window.colno(); x++)
        for (int y=0; y<window.rowno();y++)
            if (se(x,y) && window (x,y) >max) max = window (x,y);



    return max;

}
