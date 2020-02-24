#include "morph_openclose.h"

#include "morph_erode.h"
#include "morph_dilate.h"

MorphOpenClose::MorphOpenClose(PNM* img) :
    MorphologicalOperator(img), m_type(Open)
{
}

MorphOpenClose::MorphOpenClose(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv), m_type(Open)
{
}

PNM* MorphOpenClose::transform()
{
    int size = getParameter("size").toInt();;
    SE shape = (SE) getParameter("shape").toInt();
    m_type = (Type) getParameter("type").toInt();

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";




    if (m_type == Close) //erode and next dilate!
    {

        PNM* nextImg = erode(image,size,shape);

        nextImg = dilate(nextImg,size, shape);

        return nextImg;
    }

    else if (m_type == Open) //dilate and next erode!
    {
        PNM* nextImg = dilate(image,size,shape);

        nextImg = erode(nextImg,size,shape);

        return nextImg;
    }

    return 0;
}

PNM* MorphOpenClose::erode(PNM* image, int size, SE shape)
{
    MorphErode e(image, getSupervisor());
    e.setParameter("silent", true);
    e.setParameter("shape", shape);
    e.setParameter("size", size);

    return e.transform();
}

PNM* MorphOpenClose::dilate(PNM* image, int size, SE shape)
{
    MorphDilate e(image, getSupervisor());
    e.setParameter("silent", true);
    e.setParameter("shape", shape);
    e.setParameter("size", size);

    return e.transform();
}
