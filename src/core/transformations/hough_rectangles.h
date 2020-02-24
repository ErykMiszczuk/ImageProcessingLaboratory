#ifndef HOUGH_RECTANGLES_H
#define HOUGH_RECTANGLES_H

#include "transformation.h"

class HoughRectangles : public Transformation
{
public:
    explicit HoughRectangles(PNM*);
    explicit HoughRectangles(PNM*, ImageViewer*);

    virtual PNM* transform();
    static bool check90Degrees(int*, int*, int*); //sprawdzenie czy punkty są połączone liniami między którymi kąt wynosi 90'
};

#endif // HOUGH_RECTANGLES_H
