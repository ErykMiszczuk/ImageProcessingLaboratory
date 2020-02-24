#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{

    math::matrix<float> mask(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mask[i][j] = 0;
        }
    }

    mask[size / 2][size / 2] = 1;

    return mask;

}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    float weight = sum(mask);

    math::matrix<float> mirroredMask = reflection(mask);

    PNM* newImage = new PNM(width, height, image->format());

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // Get window for given pixels
            math::matrix<float> windowChannelRed = getWindow(x, y, 3, RChannel, mode);
            math::matrix<float> windowChannelGreen = getWindow(x, y, 3, GChannel, mode);
            math::matrix<float> windowChannelBlue = getWindow(x, y, 3, BChannel, mode);

            // accu for every channel
            math::matrix<float> accuRed = join(windowChannelRed, mirroredMask);
            math::matrix<float> accuGreen = join(windowChannelGreen, mirroredMask);
            math::matrix<float> accuBlue = join(windowChannelBlue, mirroredMask);

            // sums for every channel
            float sumAccuRed = Convolution::sum(accuRed);
            float sumAccuGreen = Convolution::sum(accuGreen);
            float sumAccuBlue = Convolution::sum(accuBlue);

//            qDebug() << Q_FUNC_INFO << "Convolution on x:" << x << " and y:" << y;

            // Divide sum by weight if weight is not zero
            if (weight != 0.0) {
                sumAccuRed = sumAccuRed / weight;
                sumAccuGreen = sumAccuGreen / weight;
                sumAccuBlue = sumAccuBlue / weight;
            }
            // Check if sum is less than 255 and more than -1
            if (sumAccuRed < 0) sumAccuRed = 0;
            if (sumAccuGreen < 0) sumAccuGreen = 0;
            if (sumAccuBlue < 0) sumAccuBlue = 0;
            if (sumAccuRed > 255) sumAccuRed = 255;
            if (sumAccuGreen > 255) sumAccuGreen = 255;
            if (sumAccuBlue > 255) sumAccuBlue = 255;

            QColor newPixel = QColor(sumAccuRed, sumAccuGreen, sumAccuBlue);
            newImage->setPixel(x,y, newPixel.rgb());
        }
    }

    return newImage;

}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] * B[i][j];
        }
    }

    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float summary = 0.0f;
    int size = A.rowno();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            summary += A[i][j];
        }
    }

    return summary;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    int radius = size/2;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[size - 1 - i][size - 1 - j];
        }
    }

    return C;
}
