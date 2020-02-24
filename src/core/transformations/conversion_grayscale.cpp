#include "conversion_grayscale.h"

ConversionGrayscale::ConversionGrayscale(PNM* img) :
    Transformation(img)
{
}

ConversionGrayscale::ConversionGrayscale(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* ConversionGrayscale::transform()
{
    qDebug() << Q_FUNC_INFO << "Grayscale transformation started";

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Mono)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QColor color = QColor::fromRgb(image->pixel(x,y));

                newImage->setPixel(x,y, color == Qt::white ? Qt::color1 : Qt::color0);
            }
    }
    else
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                float r = qRed(pixel);
                float g = qGreen(pixel);
                float b = qBlue(pixel);
                int avg = (r+g+b)/3;
                QColor newPixel = QColor(avg,avg,avg);
                newImage->setPixel(x,y,newPixel.rgb());
            }
    }

    qDebug() << Q_FUNC_INFO << "Grayscale transformation finished";
    return newImage;
}
