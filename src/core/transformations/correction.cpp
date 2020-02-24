#include "correction.h"

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    emit message("Correction");

    float shift  = getParameter("shift").toFloat();
    float factor = getParameter("factor").toFloat();
    float gamma  = getParameter("gamma").toFloat();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    qDebug() << Q_FUNC_INFO << "Correction started";

    int LUT[256];
    for(int i = 0; i < 256; i++) {
        float newValue = 0;
        newValue = i + shift;
        newValue = newValue * factor;
        newValue = pow(newValue, gamma);
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        LUT[i] = newValue;
    }


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QRgb pixel = image->pixel(x,y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            QColor newPixel = QColor(LUT[red], LUT[green], LUT[blue]);
            newImage->setPixel(x, y, newPixel.rgb());
        }
    }
    qDebug() << Q_FUNC_INFO << "Correction finished";
    return newImage;
}
