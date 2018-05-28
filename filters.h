#ifndef FILTERS_H
#define FILTERS_H

#include <QImage>
#include <QSize>
#include <vector>
#include <omp.h>

/**
 * @brief toShadesGrayMthr turns image to shades of grey using openMP
 * @param src[in] source image
 * @param res[out] result image
 * @param r[in] coeffitients for colors
 * @param g[in]
 * @param b[in]
 */
void toShadesGrayMthr(QImage* src, QImage* res, double r, double g, double b);
/**
 * @brief toShadesGraySthr turns image to shades of grey in single thread
 */
void toShadesGraySthr(QImage* src, QImage* res, double r, double g, double b);


/**
 * @brief gaussBlurMthr gaussian blur using openMP
 * @param src[in] source image
 * @param res[out] result image
 * @param radius[in] radius of blurring
 */
void gaussBlurMthr(QImage* src, QImage* res, int radius);
/**
 * @brief gaussBlurSthr gaussian blur in single thread
 */
void gaussBlurSthr(QImage* src, QImage* res, int radius);


/**
 * @brief motionBlurMthr motion blur using openMP
 * @param src[in] source image
 * @param res[out] result image
 * @param radius[in] radius of blurring
 * @param horizontal[in] true - horizontal blur, false - vertical blur
 */
void motionBlurMthr(QImage* src, QImage* res, int radius, bool horizontal);
/**
 * @brief motionBlurSthr motion blur in single thread
 */
void motionBlurSthr(QImage* src, QImage* res, int radius, bool horizontal);

#endif // FILTERS_H
