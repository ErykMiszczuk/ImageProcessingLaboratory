#include "edge_prewitt.h"

EdgePrewitt::EdgePrewitt(PNM*img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgePrewitt::EdgePrewitt(PNM*img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgePrewitt::prepareMatrices()
{
//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    math::matrix<float> x(3, 3);
    x(0, 0) = -1;
    x(0, 1) = 0;
    x(0, 2) = 1;
    x(1, 0) = -1;
    x(1, 1) = 0;
    x(1, 2) = 1;
    x(2, 0) = -1;
    x(2, 1) = 0;
    x(2, 2) = 1;
    g_x = x;

    math::matrix<float> y(3, 3);
    y(0, 0) = -1;
    y(0, 1) = -1;
    y(0, 2) = -1;
    y(1, 0) = 0;
    y(1, 1) = 0;
    y(1, 2) = 0;
    y(2, 0) = 1;
    y(2, 1) = 1;
    y(2, 2) = 1;
    g_y = y;
}

