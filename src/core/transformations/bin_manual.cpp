#include "bin_manual.h"

BinarizationManual::BinarizationManual(PNM* img) :
    Transformation(img)
{
}

BinarizationManual::BinarizationManual(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationManual::transform()
{
    int threshold = getParameter("threshold").toInt();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);
        for (int x=0; x<width;x++){         //get width of image
            for (int y=0; y<height; y++)    //get height of image
            {
                QRgb pixels = image-> pixel(x,y);
                int p;
                p = qGray(pixels);


                newImage -> setPixel(x,y, p < threshold? Qt::color0:Qt::color1); //make new image with new value
            }
        }

    return newImage;
}




