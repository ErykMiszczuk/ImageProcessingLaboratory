#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    // Rozciąganie.
    //najpierw szukamy lewej (min) i prawej (max) krawędzi histogramu, tzn. od jakiej wartości piksela (idąc od lewej i od prawej) zaczynają się pierwsze niezerowe wartości histogramu.

    QHash<int, int>* R = image->getHistogram()->get(Histogram::RChannel); //pobranie histogramu dla kanału R
    QHash<int, int>* G = image->getHistogram()->get(Histogram::GChannel); //pobranie histogramu dla kanału G
    QHash<int, int>* B = image->getHistogram()->get(Histogram::BChannel); //pobranie histogramu dla kanału B

    //szukanie min i max

    QHash<int, int>::const_iterator i;
    int Rminimum = 255, Bminimum = 255, Gminimum=255;
    int Bmax =0, Gmax =0, Rmax=0;
    //szukanie min i max dla R
    for (i = R-> constBegin(); i != R->constEnd(); i++) {

        if (i.key() < Rminimum && i.value()>0) {
            Rminimum = i.key();
        }
        if (i.key() > Rmax && i.value()>0) {
            Rmax = i.key();
        }
    }
    //szukanie min i max dla G
    for (i = G-> constBegin(); i != G->constEnd(); i++) {
        if (i.key() < Gminimum && i.value()>0) {
                Gminimum = i.key();
        }
        if (i.key() > Gmax && i.value()>0) {
                Gmax = i.key();
        }
    }
    //szukanie min i max dla B
    for (i = B-> constBegin(); i != B->constEnd(); i++) {
        if (i.key() < Rminimum && i.value()>0) {
                Bminimum = i.key();
        }
        if (i.key() > Bmax && i.value()>0) {
                Bmax = i.key();
        }
    }
    //pętla rozciągająca histogram poprzez użycie funkcji

     for (int a = 0; a < width; a++)
         for (int y = 0; y < height; y++)
         {
             QRgb image_pixel = image->pixel(a,y);
             int r=qRed(image_pixel);
             int g=qGreen(image_pixel);
             int b=qBlue(image_pixel);
             unsigned int stretchR=int((255.0f/float(Rmax-Rminimum)) * float(r-Rminimum));
             unsigned int stretchG=int((255.0f/float(Gmax-Gminimum)) * float(g-Gminimum));
             unsigned int stretchB=int((255.0f/float(Bmax-Bminimum)) * float(b-Bminimum));
             if (stretchR > 255) stretchR = 255;
             if (stretchG > 255) stretchG = 255;
             if (stretchB > 255) stretchB = 255;
             QRgb StrechValue = QColor(stretchR,stretchG,stretchB).rgb();
             newImage -> setPixel(a,y,StrechValue);
                                  //format() == QImage::Format_RGB32 ? StrechValue : qGray(StrechValue));
         }





    return newImage;
}
