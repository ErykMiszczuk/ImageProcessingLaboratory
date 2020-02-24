#include "edge_roberts.h"

EdgeRoberts::EdgeRoberts(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgeRoberts::EdgeRoberts(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgeRoberts::prepareMatrices()
{
//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    math::matrix<float> x(2,2);
    x(0,0) = 1;
    x(0,1) = 0;
    x(1,0) = 0;
    x(1,1) = -1;
    g_x = x;

    math::matrix<float> y(2,2);
    y(0,0) = 0;
    y(0,1) = -1;
    y(1,0) = 1;
    y(1,1) = 0;
    g_y = y;

}
