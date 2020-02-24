#include "bin_gradient.h"
using namespace::std;
#include "bin_manual.h"

BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int counter = 0, denominator=0;
    int width = image->width();
    int height = image->height();

    //PNM* newImage = new PNM(width, height, QImage::Format_Mono);
     for (int x = 0; x<width;x++){
         for (int y = 0; y<height;y++)
         {


             int G_x = qGray (image -> pixel(x<width -1? x+1 : x,y)) - qGray(image->pixel(x >0 ? x-1 :x,y));
             int G_y = qGray (image -> pixel( x , y< height-1 ? y+1 : y)) - qGray(image->pixel(x,y>0 ? y -1 :y));


             denominator = max(G_x, G_y);
              counter = max(G_x,G_y)* qGray(image->pixel(x,y));
         }


     }

     BinarizationManual * BM = new BinarizationManual(image);

     BM->setParameter("threshold", int ((float)counter/(float)denominator));
     return BM->transform();
}





//qDebug() << Q_FUNC_INFO << "Not implemented yet!";

