#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
  	emit message("Blurring...");
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);


    float tmpSum = 1.0f;
    if (normalize) {
        tmpSum = 0.0f;
        for (int i = 0; i < maskSize; i++) {
            tmpSum += tmpMask.at(i).toDouble();
        }
    }

    for (int i = 0; i < maskSize; i++) {
        mask[i / 3][i % 3] = tmpMask.at(i).toDouble() / tmpSum;
    }

    return convolute(mask, RepeatEdge);
}
