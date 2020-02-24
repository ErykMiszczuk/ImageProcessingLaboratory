#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"
#include "edge_laplacian_of_gauss.h"

#include <math.h>
#include <limits>

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::transform()
{   
//    qDebug() << Q_FUNC_INFO << "Huoght";

    int thetaDensity = getParameter("theta_density").toInt();

    int width = image->width();
    int height = image->height();

    int diameterMax = sqrt(pow(width, 2) + pow(height, 2));
    int thetaSize = 180 * thetaDensity;
    int doubleDiameter = (diameterMax * 2) + 1;

    // Convert input image to grayscale
    ConversionGrayscale* convertToGrayscale = new ConversionGrayscale(image);
    PNM* grayscaleConversion = convertToGrayscale->transform();

    // Detect edges on grayscale if skip_edge_detection is false
    if (getParameter("skip_edge_detection").toBool() == false) {
        EdgeLaplacian* laplacian = new EdgeLaplacian(grayscaleConversion);
        laplacian->setParameter("size", 3);
        grayscaleConversion = laplacian->transform();
    }

    // output image in grayscale
    PNM* outputImage = new PNM(thetaSize, doubleDiameter, QImage::Format_Grayscale8);

    math::matrix<float> hough(thetaSize, doubleDiameter);
    hough = 0.0f;

    // iterate over pixels in input image
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            int pixel = qGray(grayscaleConversion->pixel(x, y));
            if (pixel > 0) {

                for (int k = 0; k < thetaSize; k++) {
                    float theta = (k * 3.14) / thetaSize;
                    float p = (x * cos(theta)) + (y * sin(theta));
                    hough[k][p + diameterMax] = hough[k][p + diameterMax] + 1.0f;
                }

            }

        }
    }

    qDebug() << Q_FUNC_INFO << "Transforming hough matrix into image";
    float scale = 255.0f / hough.max();
    for (int x = 0; x < thetaSize; x++) {
        for (int y = 0; y < doubleDiameter; y++) {
            int gray =  hough[x][y] * scale;
            outputImage->setPixel(x, y, QColor(gray, gray, gray).rgb());
        }
    }

    return outputImage;
}
