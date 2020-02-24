#include "edge_laplacian.h"

EdgeLaplacian::EdgeLaplacian(PNM* img) :
    Convolution(img)
{
}

EdgeLaplacian::EdgeLaplacian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplacian::getMask(int, Mode)
{
    int size = getParameter("size").toInt();
    math::matrix<float> mask(size, size);

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            mask[x][y] = -1;
        }
    }

    int center = size / 2;
    mask[center][center] = ((size * size) - 1);

    return mask;
}

