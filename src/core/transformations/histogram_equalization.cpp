#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    QHash<int, int>* R = image->getHistogram()->get(Histogram::RChannel); //pobranie histogramu dla kanału R
    QHash<int, int>* G = image->getHistogram()->get(Histogram::GChannel); //pobranie histogramu dla kanału G
    QHash<int, int>* B = image->getHistogram()->get(Histogram::BChannel); //pobranie histogramu dla kanału B
    QHash<int,int>* Req = new QHash<int,int>;
    QHash<int,int>* Geq = new QHash<int,int>;
    QHash<int,int>* Beq = new QHash<int,int>;
    //każdej możliwej wartości piksela przypisujemy prawdopodobieństwo p jego wystąpienia na obrazie.
    float Radd=0,Gadd=0,Badd=0;
    for (int i=0;i<255;i++)
        {
            Radd = std::min(Radd + float (R->value(i,0))/float(width*height),1.0f);
            Req->insert(i,std::min(int(Radd*255.0f),255));
        }
    for (int i=0;i<255;i++)
        {
            Gadd = std::min(Gadd + float (G->value(i,0))/float(width*height),1.0f);
            Geq->insert(i,std::min(int(Gadd*255.0f),255));
        }
    for (int i=0;i<255;i++)
        {
            Badd = std::min(Badd + float (B->value(i,0))/float(width*height),1.0f);
            Beq->insert(i,std::min(int(Badd*255.0f),255));
        }
    for (int a=0;a<width;a++)
        for (int y=0;y<height;y++)
        {
            QRgb image_pixel = image->pixel(a,y);
            int r=qRed(image_pixel);
            int g=qGreen(image_pixel);
            int b=qBlue(image_pixel);

              QRgb EqValue = QColor(Req->value(r,0),Geq->value(g,0),Beq->value(b,0)).rgb();
              newImage -> setPixel(a,y,newImage -> format() == QImage::Format_RGB32 ? EqValue : qGray(EqValue));
        }
    return newImage;
}
