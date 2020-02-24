#ifndef HOUGH_LINES_H
#define HOUGH_LINES_H

#include "transformation.h"

class HoughLines : public Transformation
{
public:
    explicit HoughLines(PNM*);
    explicit HoughLines(PNM*, ImageViewer*);

	
    virtual PNM* transform();
    static PNM* checkLines(PNM*, int, int);
};

#endif // HOUGH_LINES_H
