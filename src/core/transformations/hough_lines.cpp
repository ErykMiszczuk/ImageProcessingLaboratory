#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"

#include <math.h>
#include <QtMath>
#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());

    int width = image->width();
    int height = image->height();

    // Edge detection
    EdgeLaplacian* laplacianEdgeDetection = new EdgeLaplacian(image);
    laplacianEdgeDetection->setParameter("size", 3);
    PNM* laplacian = laplacianEdgeDetection->transform();

    // Binarization
    BinarizationGradient* gradientBinarization = new BinarizationGradient(laplacian);
    PNM* binarization = gradientBinarization->transform();
    // Hough
    Hough* houghTransformation = new Hough(binarization);
    houghTransformation->setParameter("theta_density", 3);
    houghTransformation->setParameter("skip_edge_detection", true);
    PNM* spectrum = houghTransformation->transform();

    // Lines Drawing
    int pMax = (spectrum->height() - 1) / 2;

    PNM* linesOnPicture = new PNM(width, height, QImage::Format_Mono);
    QPainter linePainter(linesOnPicture);

    linePainter.setPen(QPen(Qt::white, 3));
    for (int x = 0; x < spectrum->width(); x++) {
        for (int y = 0; y < spectrum->height(); y++) {
            if (qGray(spectrum->pixel(x, y)) > threshold) {
                float degrees = (float)x / 3.0f;
                float theta = (degrees * 3.14) / 180;
                int p = y - pMax;

                QLineF lineToPaint;
                if ((int)degrees % 180 != 0) {
                    lineToPaint.setPoints(QPoint(0, p / sin(theta) + 1), QPoint(width, ((p - width) * cos(theta)) / (sin(theta) + 1)));
                } else {
                    lineToPaint.setPoints(QPoint(p / (sin(theta) + 1), p), QPoint(((p - width) * sin(theta)) / (cos(theta) + 1), width));
                }

                linePainter.drawLine(lineToPaint);

            }
        }
    }

    for (int x = 0; x < width; x++) {
         for (int y = 0; y < height; y++) {
             if (qGray(linesOnPicture->pixel(x, y)) > 0 && (drawWholeLines || qGray(binarization->pixel(x, y)) > 0)) {
                 newImage->setPixel(x, y, qRgb(255, 0, 0));
             } else {
                 newImage->setPixel(x, y, image->pixel(x, y));
             }
         }
     }

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return newImage;
}


PNM* HoughLines::checkLines(PNM * binarization, int threshold, int lineWidth)

{
    int width = binarization->width();
    int height = binarization->height();

    // Hough
    Hough* houghTransformation = new Hough(binarization);
    houghTransformation->setParameter("theta_density", 3);
    houghTransformation->setParameter("skip_edge_detection", true);
    PNM* spectrum = houghTransformation->transform();

    // Lines Drawing
    int pMax = (spectrum->height() - 1) / 2;

    PNM* linesOnPicture_ = new PNM(width, height, QImage::Format_Mono);
    QPainter linePainter(linesOnPicture_);

    linePainter.setPen(QPen(Qt::white, 3));

    for (int x = 0; x < spectrum->width(); x++) {
        for (int y = 0; y < spectrum->height(); y++) {
            if (qGray(spectrum->pixel(x, y)) > threshold) {
                float degrees = (float)x / 3.0f;
                float theta = (degrees * 3.14) / 180;
                int p = y - pMax;

                QLineF lineToPaint;
                if ((int)degrees % 180 != 0) {
                    lineToPaint.setPoints(QPoint(0, p / sin(theta) + 1), QPoint(width, ((p - width) * cos(theta)) / (sin(theta) + 1)));
                } else {
                    lineToPaint.setPoints(QPoint(p / (sin(theta) + 1), p), QPoint(((p - width) * sin(theta)) / (cos(theta) + 1), width));
                }

                linePainter.drawLine(lineToPaint);

            }
        }
    }
    return linesOnPicture_;
}

