#include "edge_laplacian_of_gauss.h"

#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();

    math::matrix<float> mask(size, size);
    int radius = size / 2;

    for (int i = 0; i < size; i += 1) {
        for (int j = 0; j < size; j += 1) {
            mask(i, j) = getLoG(i - radius, j - radius, sigma);
        }
    }

    return mask;
}

float EdgeLaplaceOfGauss::getLoG(int x, int y, float s)
{
    return (powf(x, 2) + powf(y, 2) - 2) / powf(s, 2);
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}
