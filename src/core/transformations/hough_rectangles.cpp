#include "hough_rectangles.h"
#include "hough_lines.h"
#include "corner_harris.h"
#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "conversion_grayscale.h"

#include <QPainter>
#include <QtMath>

HoughRectangles::HoughRectangles(PNM* img) :
    Transformation(img)
{
}

HoughRectangles::HoughRectangles(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughRectangles::transform()
{

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";


    //int thetaDensity = 3;
    int width = image -> width();
    int height = image -> height();


    EdgeLaplacian laplacian(image);

    laplacian.setParameter("size",3);

    PNM* lapcian_img = laplacian.transform();

    BinarizationGradient gradient(lapcian_img);
    PNM* bin_img = gradient.transform();
    PNM* lines_on_img = HoughLines::checkLines(bin_img, 150, 3);

    //dane z zadania 12 do wykrywania narożników
    CornerHarris harris(image);
    harris.setParameter("sigma", 1.0f);
    harris.setParameter("k", 0.05f);
    harris.setParameter("threshold", 30000000);
    harris.setParameter("sigma_weight", 0.76f);

    PNM* harris_corner_detect = harris.transform();



    QVector<int*> detect_corners;
    for(int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            if (qGray (lines_on_img -> pixel(x,y))> 0 && qGray(harris_corner_detect -> pixel(x,y)) > 0)
            {
                int* corner = new int[2] {x, y};
                detect_corners.push_back(corner);
            }
        }
    }


    ConversionGrayscale convgray(image);
    PNM gray_img = convgray.transform() -> convertToFormat(QImage::Format_RGB32);

    PNM* newImage = new PNM(gray_img.copy());

    QPainter painter (newImage);

    painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));

     for (int i = 0; i < detect_corners.length(); i++) {
         for (int j = i + 1; j < detect_corners.length(); j++) {
             for (int k = j + 1; k < detect_corners.length(); k++) {
                 if (check90Degrees(detect_corners[i], detect_corners[j], detect_corners[k])) {
                     for (int l = k + 1; l < detect_corners.length(); l++) {
                         if (
                             check90Degrees(detect_corners[i], detect_corners[j], detect_corners[l]) &&
                             check90Degrees(detect_corners[i], detect_corners[k], detect_corners[l])
                          ) {
                             painter.drawPoint(detect_corners[i][0], detect_corners[i][1]);
                             painter.drawPoint(detect_corners[j][0], detect_corners[j][1]);
                             painter.drawPoint(detect_corners[k][0], detect_corners[k][1]);
                             painter.drawPoint(detect_corners[l][0], detect_corners[l][1]);
                         }
                     }
                 }
             }
         }
     }

     return newImage;
 }


bool HoughRectangles::check90Degrees(int* first_corner, int* second_corner, int* third_corner)
{
        float dist_f_s = sqrt(pow(abs(first_corner[0] - second_corner[0]), 2) + pow(abs(first_corner[1] - second_corner[1]), 2));
        float dist_f_t = sqrt(pow(abs(first_corner[0] - third_corner[0]), 2) + pow(abs(first_corner[1] - third_corner[1]), 2));
        float dist_s_t = sqrt(pow(abs(second_corner[0] - third_corner[0]), 2) + pow(abs(second_corner[1] - third_corner[1]), 2));

        float x,y,z;
        if(dist_f_t < 10 || dist_f_s < 10 || dist_s_t < 10)
        {
            return false;
        }
        if (dist_f_s < dist_s_t && dist_f_t < dist_s_t)
        {
                    x = dist_f_s;
                    y = dist_f_t;
                    z = dist_s_t;
        }
        else if (dist_f_s < dist_f_t && dist_s_t < dist_f_t)
        {
                   x = dist_f_s;
                   y = dist_s_t;
                   z = dist_f_t;

        }
        else if (dist_f_t < dist_f_s && dist_s_t < dist_f_s)
        {
                    x = dist_f_s;
                    y = dist_s_t;
                    z = dist_f_t;

        }
        else
        {
            return false;
        }

        //Return the floor of the value v.
        //The floor is the largest integer that is not greater than v. For example, if v is 41.2, then the floor is 41.
        return qFabs(( (x*x) + (y*y))/ (z*z) - 1.0f) <0.02f;


}

