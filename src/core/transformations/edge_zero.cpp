#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

#include "edge_laplacian.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();

    int size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();

    bool blackPixel = true;

    PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);
    EdgeLaplaceOfGauss* gauss = new EdgeLaplaceOfGauss(image);
//    qDebug() << Q_FUNC_INFO << "Mask size" << size;
    PNM* laplaceanGaussed = convolute(gauss->getMask(size, RepeatEdge), RepeatEdge);
//    EdgeLaplacian* laplacian = new EdgeLaplacian(image);
//    PNM* laplaceanGaussed = convolute(laplacian->getMask(size,RepeatEdge), RepeatEdge);

    // Buil in min max functions dont work (of course) so thats why we need to find min max manualy
    int v0 = 128;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            math::matrix<float> window(size, size);
            window = getWindow(i, j, size, LChannel, RepeatEdge);
            int minVal = 255;
            for(int x = 0; x < size; x++) {
                for(int y = 0; y < size; y++) {
                    if(window(x, y) < minVal) {
                        minVal = window(x, y);
                    }
                }
            }
            int maxVal = 0;
            for(int x = 0; x < size; x++) {
                for(int y = 0; y < size; y++) {
                    if(window(x, y) > maxVal) {
                        maxVal = window(x, y);
                    }
                }
            }
            if((minVal < (v0 - t)) && (maxVal > (v0 + t))) {
                QRgb pixel = laplaceanGaussed->pixel(i, j);
//                int r = qRed(pixel);
//                int g = qGreen(pixel);
//                int b = qBlue(pixel);
                newImage->setPixel(i, j, QColor(255,255,255).rgb());
                blackPixel = false;
            } else {
                newImage->setPixel(i, j, QColor(0,0,0).rgb());
                blackPixel = true;
            }
            qDebug() << Q_FUNC_INFO << "Should pixel be black" << blackPixel;
        }
    }

    return newImage;
}

