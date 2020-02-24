#include "corner_harris.h"
#include "bin_gradient.h"
#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();

    // PNM* newImage = new PNM(width, height, QImage::Format_Mono);
   // qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    // macierze zerowe o wymiarach obrazu wejsciowego.
     math::matrix<float> iyy (width, height);
     iyy = 0;
     math::matrix<float> ixx (width, height);
     ixx = 0;
     math::matrix<float> ixy (width, height);
     ixy = 0;
     math::matrix<int> corner_candidates (width,height);
     corner_candidates = 0;
     math::matrix<float> corner_nonmax_suppress (width, height);
     corner_nonmax_suppress = 0;

     // Dokonaj konwersji obrazu wejsciowego na skale odcieni szarosci.
     //BinarizationGradient bGradient(image);
     //PNM* gray_image = bGradient.transform();
     ConversionGrayscale* convertToGrayscale = new ConversionGrayscale(image);
     PNM* grayscaleConversion = convertToGrayscale->transform();


     // Rozmyj otrzymany obraz filtrem Gaussa 3×3 z parametrem σ=1.6.
     BlurGaussian bGaussian (grayscaleConversion);
     bGaussian.setParameter("size", 3);
     bGaussian.setParameter("sigma", 1.6f);
     PNM* blure_image = bGaussian.transform();

     //Oblicz gradienty kierunkowe Gx i Gy operatorem Sobela przy pomocy metod rawHorizontalDetection() i rawVerticalDetection().
     //skopiowane z edge_canny.cpp
     EdgeSobel *eSobel = new EdgeSobel (blure_image);
     math::matrix<float> Gx = *eSobel -> rawHorizontalDetection();
     math::matrix<float> Gy = *eSobel -> rawVerticalDetection();

     for (int x=0;x<width;x++)
     {
         for (int y=0;y<height;y++)
         {
             ixx[x][y] = Gx[x][y] * Gx[x][y];
             iyy[x][y] = Gy[x][y] * Gy[x][y];
             ixy[x][y] = Gx[x][y] * Gy[x][y];
         }
     }


    //Dla każdego piksela (i,j), jeżeli piksel (i,j) nie leży na brzegu obrazu, to:
     for (int x = 1; x < width - 1; x++) {
             for (int y = 1; y < height - 1; y++) {
                 float sigma_ixx = 0;
                 float sigma_ixy = 0;
                 float sigma_iyy = 0;

                 for (int xx = -1; xx < 2; xx++) {
                     for (int yy = -1; yy < 2; yy++) {
                         float gauss = BlurGaussian::getGauss(xx, yy, sigma);
                         sigma_ixx += ixx[x + xx][y + yy] * gauss;
                         sigma_ixy += ixy[x + xx][y + yy] * gauss;
                         sigma_iyy += iyy[x + xx][y + yy] * gauss;
//                         qDebug() << Q_FUNC_INFO << "sigma_ixx: " << sigma_ixx << "sigma_ixy: " << sigma_ixy<< "sigma_iyy: " << sigma_iyy;
                     }
                 }
                 sigma_ixx /= sigma_weight;
                 sigma_ixy /= sigma_weight;
                 sigma_iyy /= sigma_weight;
                 float det_H = sigma_ixx * sigma_iyy - sigma_ixy * sigma_ixy;
                 float thresholdH = sigma_ixx + sigma_iyy;
                 float r = det_H - k_param * (thresholdH * thresholdH);
//                 qDebug() << Q_FUNC_INFO << "det_H: " << det_H << "thresholdH: " << thresholdH<< "r: " << r;
                 if (r > threshold) {
                     corner_candidates[x][y] = r;
                 }
             }
         }


     // search:=true
     bool search = true;

    // tak długo jak search==true wykonuj:

     while (search)
     {
         search = false;
         for (int x = 1; x < width-1; x++)
         {
             for (int y = 1; y < height-1; y++)
             {
                 if(     corner_candidates[x][y] > corner_candidates[x - 1][y - 1] &&
                         corner_candidates[x][y] > corner_candidates[x - 1][y] &&
                         corner_candidates[x][y] > corner_candidates[x + 1][y + 1] &&
                         corner_candidates[x][y] > corner_candidates[x + 1][y] &&
                         corner_candidates[x][y] > corner_candidates[x][y + 1] &&
                         corner_candidates[x][y] > corner_candidates[x + 1][y - 1] &&
                         corner_candidates[x][y] > corner_candidates[x][y - 1] &&
                         corner_candidates[x][y] > corner_candidates[x - 1][y + 1])
                 {
                     corner_nonmax_suppress[x][y] = corner_candidates[x][y];
                 }
                 else
                 {
                     if (corner_candidates[x][y] > 0)
                     {
                         search = true;
                     }
                     corner_nonmax_suppress[x][y] = 0;
                 }
                 corner_candidates[x][y] = corner_nonmax_suppress[x][y];
             }
         }
     }

     //Dla każdego piksela (i,j)
     PNM* newImage = new PNM(width, height, QImage::Format_Mono);

     for (int s = 0; s < width; s++) {
            for (int t = 0; t < height; t++) {
                if(corner_candidates[s][t] == 0) {
                    newImage->setPixel(s,t,0);
                } else {
                    newImage->setPixel(s,t,1);
                }
            }
     }




    return newImage;
}
