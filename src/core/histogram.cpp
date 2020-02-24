#include "histogram.h"

#include <QDebug>
#include <QPainter>

#include <cmath>

Histogram::Histogram(QImage* image)
{
    R = new QHash<int, int>;
    G = new QHash<int, int>;
    B = new QHash<int, int>;
    L = new QHash<int, int>;
    generate(image);
}

Histogram::~Histogram()
{
    delete R;
    delete G;
    delete B;
    delete L;
}

//wyświetlenie histogramu w widoku
QTextStream& HistogramOut()
{
    static QTextStream histogram(stdout);
    return histogram;
}


void Histogram::generate(QImage* image)
{

    //1. sprawdzić jaką ma wartość na każdym z 4 kanałów,

    for(int i=0;i<image->width();i++)
        for (int x=0;x<image->height();x++)
        {
            QRgb image_in_pixels = image -> pixel(i,x); //tablica pixeli z obrazu
            int r=qRed(image_in_pixels);       // Pobranie wartości z zakresu 0-255 z kanału R
            int g=qGreen(image_in_pixels);     // Pobranie wartości z zakresu 0-255 z kanału G
            int b=qBlue(image_in_pixels);      // Pobranie wartości z zakresu 0-255 z kanału B
            int l=qGray(image_in_pixels);      // Pobranie wartości z zakresu 0-255 z kanału L

           //2. zwiększyć o 1 odpowiednią wartość w każdej tablicy haszującej.

            int cr = r;
            int crr = R->value(cr);
            crr++;
            R -> insert(cr,crr);
            int cg = g;
            int cgg = G->value(cg);
            cgg++;
            G -> insert(cg,cgg);
            int cb = b;
            int cbb = B->value(cb);
            cbb++;
            B -> insert(cb,cbb);

        }
    for(int i=0;i<256;i++){

               HistogramOut() << R->value(i) << endl;

           }

}

/** Returns the maximal value of the histogram in the given channel */
int Histogram::maximumValue(Channel selectedChannel = RGB)
{
    int m = 0;

    for(int x = 0;x<256;x++)
        {
            if (R->value(x)>m)
                {
                    m = R-> value(x);
                }
            if (G->value(x)>m)
                {
                    m = G-> value(x);
                }
            if (B->value(x)>m)
                {
                    m = B-> value(x);
                }
            if (L->value(x)>m)
                {
                    m = L-> value(x);
                }
        }

     return m;
}


/** Returns a pointer to the given channel QHash<int, int> **/
QHash<int, int>* Histogram::get(Channel channel = LChannel)
{
    if (channel==LChannel) return L;
    if (channel==RChannel) return R;
    if (channel==GChannel) return G;
    if (channel==BChannel) return B;

    return 0;
}

/**
 *  Returns a 255 by 100 QImage containing a Histogram for the given channel.
 *  The background is transparent (Alpha 0, RGB=255) */
QImage Histogram::getImage(Channel channel = LChannel, QBrush pen = Qt::gray)
{
    // Create blank QImage and fill it with transparent background:
    QImage histImage(255, 100, QImage::Format_ARGB32);
    histImage.fill(0);
    QPainter painter(&histImage);
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::transparent);
    painter.drawRect(0,0,255,100);

    // Calculate the aspect ratio using the maximal value of the color histograms
    int maximum = (channel == LChannel ? maximumValue(LChannel) :  maximumValue(RGB));
    float ratio = 100.0/float(maximum);

    // Preparing the painter:
    painter.setBrush(pen);
    painter.setPen(pen.color());

    int h;
    // Draw histogram
    QHash<int, int>* hist = get(channel);
    QHash<int, int>::const_iterator cit = hist->begin();

    while (cit != hist->end())
    {
        h = 100 - floor(ratio*cit.value());
        painter.drawLine(cit.key(), h, cit.key(), 100);
        ++cit;
    }

    return histImage;
}
