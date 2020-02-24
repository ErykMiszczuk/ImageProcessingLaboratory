#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    //    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
        math::matrix<float> x(3, 3);
        x(0, 0) = -1;
        x(0, 1) = -2;
        x(0, 2) = -1;
        x(1, 0) = 0;
        x(1, 1) = 0;
        x(1, 2) = 0;
        x(2, 0) = 1;
        x(2, 1) = 2;
        x(2, 2) = 1;
        g_x = x;

        math::matrix<float> y(3, 3);
        y(0, 0) = -1;
        y(0, 1) = 0;
        y(0, 2) = 1;
        y(1, 0) = -2;
        y(1, 1) = 0;
        y(1, 2) = 2;
        y(2, 0) = -1;
        y(2, 1) = 0;
        y(2, 2) = 1;
        g_y = y;
}

math::matrix<float>* EdgeSobel::rawHorizontalDetection()
{
    int width = this->image->width(), height = this->image->height();

    qDebug() << Q_FUNC_INFO << "width: " << width << "height: " << height;

   math::matrix<float> x_grad(width, height);

    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
//            qDebug() << Q_FUNC_INFO << "Current pixel x: " << x << "and y: " << y;
            math::matrix<float> window(3,3);
            window = getWindow(x,y,3,LChannel,RepeatEdge);
            x_grad[x][y]= sum(join(g_x, window));
        }
    }


    math::matrix<float>* x_gradient = new math::matrix<float>(width, height);

    *x_gradient = x_grad;
    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    int width = this->image->width(), height = this->image->height();

    math::matrix<float> y_grad(width, height);

    qDebug() << Q_FUNC_INFO << "width: " << width << "height: " << height;

    for (int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            y_grad[x][y]= sum(join(g_y, getWindow(x,y,3,LChannel,RepeatEdge)));
        }
    }
    math::matrix<float>* y_gradient = new math::matrix<float>(width, height);

    *y_gradient = y_grad;

    return y_gradient;
}
