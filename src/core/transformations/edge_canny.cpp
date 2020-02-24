#include <iostream>
#include "edge_canny.h"
#include <algorithm>
#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"
#include <cstdio>
#include <cmath>

using namespace std;
EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    //PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);
   // qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    // conversion to gray scale
    ConversionGrayscale *gs = new ConversionGrayscale (image);
    PNM* grayScaleConversion = gs -> transform();

    // gauss filtr 3x3 and sigm: 1.6
    BlurGaussian *bGaussian = new BlurGaussian(grayScaleConversion);
    bGaussian -> setParameter("size", 3);
    bGaussian -> setParameter("sigma", 1.6);
    PNM* bluredGaussian = bGaussian->transform();

    // directional gradients
    EdgeSobel *eSobel = new EdgeSobel (image);
    math::matrix<float> Gx = *eSobel -> rawHorizontalDetection();
    math::matrix<float> Gy = *eSobel -> rawVerticalDetection();

    //m and theta
    enum GradientDirection {Horizontal, Vertical, DiagonalFirst, DiagonalSecond}; //represents the possible directions of the gradient color
    math::matrix<float> m(width,height);
    math::matrix<GradientDirection> gradient(width,height);

    // sprawdzanie w ktorym z 4 kierunkow skierowany jest gradient piksela (i,j)

    for(int x=0;x<width;x++)
    {
        for (int y=0;y<height;y++)
        {
            m[x][y] = sqrt(pow(Gx[x][y], 2) + pow(Gy[x][y], 2));
            double t = atan(Gy[x][y] / Gx[x][y]); // t - Theta(i,j)

//            gradient[x][y] = (t>=157.5 || t<22.5 ) ? GradientDirection::Horizontal : (t <67.5 && t >= 22.5) ? GradientDirection::DiagonalFirst : (t >= 67.5 && t <112.5) ? GradientDirection::Vertical: GradientDirection::DiagonalSecond;
            if ((t >= 157 && t <= 180) || (t <= 22.5 && t >= 0)) gradient[x][y] = GradientDirection::Horizontal;
            else if (t <67.5 && t >= 22.5) gradient[x][y] = GradientDirection::DiagonalFirst;
            else if (t >= 67.5 && t <112.5) gradient[x][y] = GradientDirection::Vertical;
            else gradient[x][y] = GradientDirection::DiagonalSecond;
        }
    }

    //wybieranie sasiadow piksela (i,j)

    PNM* suppression = new PNM(width,height, QImage::Format_Grayscale8);
    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            float neighborPixel1 = 0, neighborPixel2 = 0;

            switch (gradient[x][y])
            {
            case  GradientDirection::Horizontal:
                neighborPixel1 = m[x][max(y-1,0)];
                neighborPixel2 = m[x][min(y+1, height -1)];
                break;
            case GradientDirection::DiagonalFirst:
                neighborPixel1 = m[min(x+1, width-1)][max(y-1,0)];
                neighborPixel2 = m[min(x+1, width-1)][min(y+1, height-1)];
                break;
            case GradientDirection::Vertical:
                neighborPixel1 = m[max(x-1,0)][y];
                neighborPixel2 = m[min(x+1, width -1 )][y];
                break;
            case GradientDirection::DiagonalSecond:
                neighborPixel1 = m[max(x-1,0)][max(y-1,0)];
                neighborPixel2 = m[min (x+1, width -1)][min(y+1,height)];
                break;
            }

            if ((m[x][y] > neighborPixel1 && m[x][y] > neighborPixel2) && (m[x][y] > upper_thresh)) suppression -> setPixel(x, y, m[x][y]);

        }
    }

    //progowanie z histereza

    PNM* hysteresis = new PNM(width, height, QImage::Format_Grayscale8);

    for (int x = 0; x < width; x++)
   {
        for (int y = 0; y < height; y++)
        {

            for (int find_direction = 0; find_direction < 2; find_direction++)
            {
                if (qGray(suppression->pixel(x,y))>0)
                {
                    bool find = true;
                    int pixel1 = x;
                    int pixel2 = y;

                    do {
                        int neighbor1, neighbor2;

                        switch (gradient[pixel1][pixel2])
                        {
                         case GradientDirection::Vertical:
                            neighbor1 = find_direction ? pixel1 - 1: pixel1 + 1;
                            neighbor2 = pixel2;
                            break;
                         case GradientDirection::Horizontal:
                            neighbor1 = pixel1;
                            neighbor2 = find_direction ? pixel2 -1 : pixel2 +1;
                            break;
                         case GradientDirection::DiagonalFirst:
                            neighbor1 = find_direction ? pixel1 + 1 :pixel1 -1;
                            neighbor2 = find_direction ? pixel2 -1 : pixel2 +1;
                            break;
                         case GradientDirection::DiagonalSecond:
                            neighbor1 = find_direction ? pixel1 - 1 : pixel1 +1;
                            neighbor2 = find_direction ? pixel2 -1 : pixel2 +1;
                            break;
                        }

                        if (suppression->pixel(neighbor1, neighbor2) < lower_thresh) break;
                        if (gradient[neighbor1][neighbor2] != gradient[x][y]) break;
                        //z wylaczeniem sasiada-piksela

                        for (int i = -1; i < 2; i += 2)
                        {
                            for (int j = -1; j < 2; j += 2)
                            {
                                if (i == 0 && j == 0) continue;
                                if (neighbor1 + i == neighbor2 + j == pixel2 && pixel1) continue;
                                if (neighbor1 + i < 0 || neighbor1 + i > width -1 || neighbor2 + j < 0 || neighbor2 + j > height-1) continue;
                                if (m[neighbor1][neighbor2] <= m[neighbor1 + i][neighbor2 + j]) find = false;
                            }
                        }

                    if (find){ hysteresis -> setPixel(neighbor1,neighbor2,255);}
                    pixel1 = neighbor1;
                    pixel2 = neighbor2;
                   } while ( ((pixel1 < width) && (pixel2 < height)) && ((pixel1 > 0) && (pixel2 > 0)) && find);
                 }
            }
           }
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int supp = qGray(suppression->pixel(x,y));
            int hys = qGray(hysteresis->pixel(x,y));
            qDebug() << Q_FUNC_INFO << "width: " << width << "height: " << height << "Suppression: " << supp << "Hysteresis: " << hys;
//            if (supp > 0)
//            {
//                newImage->setPixel(x,y, PIXEL_VAL_MAX);
//            }
//            else if (hys > 0)
//            {
//                newImage->setPixel(x,y, PIXEL_VAL_MAX);
//            }
//            else
//            {
//                newImage->setPixel(x,y, PIXEL_VAL_MIN);
//            }
            if ((supp + hys) > 0) {
                newImage->setPixel(x,y, PIXEL_VAL_MIN);
            }
        }
    }

    return newImage;

}
