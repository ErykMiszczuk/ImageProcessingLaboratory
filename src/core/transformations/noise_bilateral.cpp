#include "noise_bilateral.h"
#include <math.h>

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    sigma_d = getParameter("sigma_d").toInt();
    sigma_r = getParameter("sigma_r").toInt();
    radius = sigma_d;

    int valForRedChannel = 0;
    int valForBlueChannel = 0;
    int valForGreenChannel = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            valForRedChannel = NoiseBilateral::calcVal(x, y, RChannel);
            valForBlueChannel = NoiseBilateral::calcVal(x, y, BChannel);
            valForGreenChannel = NoiseBilateral::calcVal(x, y, GChannel);

            QColor newPixel = QColor(
                                        valForRedChannel,
                                        valForGreenChannel,
                                        valForBlueChannel
                                    );
            newImage->setPixel(x,y, newPixel.rgb());
        }
    }

    return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
    int size = sigma_d * 2 + 1;
    math::matrix<float> window(size, size);
    window = Transformation::getWindow(x, y, size, channel, RepeatEdge);

    float sumInCounter = 0.0f;
    float sumInDenominator = 0.0f;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float valColorCloseness = NoiseBilateral::colorCloseness(window(i, j), window(sigma_d + 1, sigma_d + 1));
            float valSpatialCloseness = NoiseBilateral::spatialCloseness(QPoint(x, y), QPoint(x + i - sigma_d, y + j - sigma_d));
            sumInCounter += window(i, j) * valColorCloseness * valSpatialCloseness;
            sumInDenominator += valColorCloseness * valSpatialCloseness;
        }
    }

    return sumInCounter/sumInDenominator;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    float valuesSubrtactedAndSqared = pow(val1 - val2, 2);
    float doubleSquareSigma = 2 * (sigma_r * sigma_r);

    return exp(-1 * (valuesSubrtactedAndSqared / doubleSquareSigma));
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
    float squareXPoints = pow(point1.x() - point2.x(),2);
    float squareYPoints = pow(point1.y() - point2.y(),2);
    float sumForXAndYPoints = squareXPoints + squareYPoints;
    float doubleSquareSigma = 2 * (sigma_d * sigma_d);

    return exp(-1 * (sumForXAndYPoints / doubleSquareSigma));
}
