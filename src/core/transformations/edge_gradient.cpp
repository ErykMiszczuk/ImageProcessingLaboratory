#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    PNM* imageX = horizontalDetection();
    PNM* imageY = verticalDetection();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // Values of ImageX
            QRgb imageXpixel = imageX->pixel(x,y);

            int imageXr = qRed(imageXpixel);
            int imageXg = qGreen(imageXpixel);
            int imageXb = qBlue(imageXpixel);

            // Values of ImageY
            QRgb imageYpixel = imageY->pixel(x,y);

            int imageYr = qRed(imageYpixel);
            int imageYg = qGreen(imageYpixel);
            int imageYb = qBlue(imageYpixel);

            // Values for new image
            int newImageR = sqrt((imageXr * imageXr)+(imageYr * imageYr));
            int newImageG = sqrt((imageXg * imageXg)+(imageYg * imageYg));
            int newImageB = sqrt((imageXb * imageXb)+(imageYb * imageYb));

//            qDebug() << Q_FUNC_INFO << "Edge detection on x:" << x << " and y:" << y <<" rgb: "<< newImageR << " " << newImageG << " " << newImageB;

            QColor newPixel = QColor(newImageR, newImageG, newImageB);
            newImage->setPixel(x,y, newPixel.rgb());
        }
    }

    return newImage;
//    return imageY;
}

