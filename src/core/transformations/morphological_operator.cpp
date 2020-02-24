#include "morphological_operator.h"

MorphologicalOperator::MorphologicalOperator(PNM* img) :
    Transformation(img)
{
}

MorphologicalOperator::MorphologicalOperator(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

// abstract
const int MorphologicalOperator::morph(math::matrix<float>, math::matrix<bool>)
{
    return 0;
}

math::matrix<bool> MorphologicalOperator::getSE(int size, SE shape)
{


    switch (shape)
    {
    case Square:    return seSquare(size);
    case Cross:     return seCross(size);
    case XCross:    return seXCross(size);
    case VLine:     return seVLine(size);
    case HLine:     return seHLine(size);
    default:        return seSquare(size);
    }
}

// sieze always is >=3

math::matrix<bool> MorphologicalOperator::seSquare(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x<size; x++)
        for (int y = 0; y<size;y++) ret[x][y] = true;



    return ret;
}

math::matrix<bool> MorphologicalOperator::seCross(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x<size; x++)
        for (int y = 0; y<size;y++) ret[x][y] = false;



    for (int x = 0; x<size; x++){
        for (int y = 0; y<size;y++) {
            if(((y > (size/2) || y < (size/2)) && x == (size/2)) || y == (size/2))
            {
                ret[x][y]=true;
            }
        }
}

    return ret;
}

math::matrix<bool> MorphologicalOperator::seXCross(int size)
{
    math::matrix<bool> ret(size, size);

   // qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x = 0; x<size; x++)
        for (int y = 0; y<size;y++) ret[x][y] = false;



    for (int x = 0;x<size;x++) ret[x][x] = true;

    int h = size - 1;
    for (int y = 0; y< size; y++) { ret[y][h] = true; h -= h; }



    return ret;
}

math::matrix<bool> MorphologicalOperator::seVLine(int size)
{
    math::matrix<bool> ret(size, size);



    for (int x = 0; x<size; x++)
        for (int y = 0; y<size;y++) ret[x][y] = false;


    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x = 0; x < size; x++ ) ret[x][size/2] = true;

    return ret;
}

math::matrix<bool> MorphologicalOperator::seHLine(int size)
{
    math::matrix<bool> ret(size, size);

    // qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x = 0; x<size; x++)
        for (int y = 0; y<size;y++) ret[x][y] = false;


    for (int x = 0; x< size; x++) ret[size/2][x]=true;
    return ret;
}

PNM* MorphologicalOperator::transform()
{  
    int size  = getParameter("size").toInt();
    SE  shape = (MorphologicalOperator::SE) getParameter("shape").toInt();

    PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    math::matrix<bool> sizeShape = getSE(size,shape);
    int w = image ->width();
    int h = image -> height();


    if (image -> format() == QImage :: Format_Grayscale8)
    {
        for (int x=0; x<w; x++ )
             {
                for (int y=0; y<h; y++)
                    {
                        math::matrix<float> window(size, size);

                        window = getWindow(x,y,size,LChannel,RepeatEdge);

                        int z = morph(window, sizeShape);

                        newImage -> setPixel(x,y, QColor(z,z,z).rgb());
                    }
             }
    }


    else
    {
        for (int x = 0; x < w; x++)
        {
            for (int y=0; y<h; y++)
            {



                    math::matrix<float> windowR(size,size);
                    math::matrix<float> windowB(size,size);
                    math::matrix<float> windowG(size,size);


                     windowR = getWindow(x,y,size,RChannel,RepeatEdge);
                     windowB = getWindow(x,y,size,BChannel,RepeatEdge);
                     windowG = getWindow(x,y,size,GChannel,RepeatEdge);

                    int setR;
                    int setG;
                    int setB;

                    setR = morph(windowR, sizeShape);
                    setG = morph(windowG, sizeShape);
                    setB = morph(windowB, sizeShape);




                newImage -> setPixel(x,y,QColor (setR,setG,setB).rgb());



            }
        }

    }





    return newImage;
}
