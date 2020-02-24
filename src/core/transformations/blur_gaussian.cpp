#include "blur_gaussian.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mask[i][j] = getGauss(i, j, sigma);
        }
    }

    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{

    int pixelX = x - 1;
    int pixelY = y - 1;

    float sigmapow = sigma * sigma;
    float gauss = (1/(2 * 3.14 * sigmapow)) * exp(-1 * (pow(pixelX,2) + pow(pixelY,2)) / 2 * sigmapow);

    return gauss;
}

