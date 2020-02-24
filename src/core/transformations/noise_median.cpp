#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    int medianForRedChannel = 0;
    int medianForBlueChannel = 0;
    int medianForGreenChannel = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            medianForRedChannel = NoiseMedian::getMedian(x, y, RChannel);
            medianForBlueChannel = NoiseMedian::getMedian(x, y, BChannel);
            medianForGreenChannel = NoiseMedian::getMedian(x, y, GChannel);

            QColor newPixel = QColor(
                                        medianForRedChannel,
                                        medianForGreenChannel,
                                        medianForBlueChannel
                                    );
            newImage->setPixel(x,y, newPixel.rgb());
        }
    }

    return newImage;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();
    int size = 1;
    size = (radius * 2) + 1;
    math::matrix<float> window(size, size);
    window = Transformation::getWindow(x, y, size, channel, RepeatEdge);

    std::vector<int> v;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            v.push_back(window[x][y]);
        }
    }

    sort(v.begin(), v.end());

    return v.at(radius + 1);
}
