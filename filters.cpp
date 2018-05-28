#include "filters.h"

void toShadesGrayMthr(QImage* src, QImage* res, double r, double g, double b)
{
    QSize size = src->size();

    if(r + g + b == 0)
    {
        r = 11.0;
        g = 16.0;
        b = 5.0;
    }

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int N_thr = omp_get_num_threads();

        for(int i = thread_id; i < size.width(); i += N_thr)
        {
            for(int j = 0; j < size.height(); j++)
            {
                QRgb src_color = src->pixel(i, j);
                int gr = (int)round((qRed(src_color) * r + qGreen(src_color) * g + qBlue(src_color) * b) / (r + g + b));

                QRgb res_color = qRgb(gr, gr, gr);

#pragma omp critical
                res->setPixel(i, j, res_color);
            }
        }
#pragma omp barrier
    }
}

void toShadesGraySthr(QImage* src, QImage* res, double r, double g, double b)
{
    QSize size = src->size();

    if(r + g + b == 0)
    {
        r = 11.0;
        g = 16.0;
        b = 5.0;
    }

    for(int i = 0; i < size.width(); i++)
    {
        for(int j = 0; j < size.height(); j++)
        {
            QRgb src_color = src->pixel(i, j);
            int gr = (int)round((qRed(src_color) * r + qGreen(src_color) * g + qBlue(src_color) * b) / (r + g + b));

            QRgb res_color = qRgb(gr, gr, gr);
            res->setPixel(i, j, res_color);
        }
    }

}

//counting coefficients for gaussian blur
void gaussCoefs(int radius, std::vector<double> &coefs)
{

    coefs.resize(6 * radius);
    double s{0};

    for(int i = 0; i < 6 * radius; i++)
    {
        coefs[i] = exp(-pow((i - 3.0 * radius)/radius, 2)/2);
        s += coefs[i];
    }

    for(int i = 0; i < 6 * radius; i++)
    {
        coefs[i] /= s;
    }
}

//getting QRgb color from doubles
QRgb getColor(double r, double g, double b)
{
    int ir = (int)round(r);
    int ig = (int)round(g);
    int ib = (int)round(b);

    QRgb res = qRgb(ir, ig, ib);
    return res;
}

void gaussBlurMthr(QImage* src, QImage* res, int radius)
{
    if(radius == 0)
    {
        return;
    }

    QSize size = src->size();

    std::vector<double> coeffs;
    gaussCoefs(radius, coeffs);
    QImage* tempImg = new QImage(*src);

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int N_thr = omp_get_num_threads();

        for(int j = thread_id; j < size.height(); j += N_thr)
        {
            for(int i = 0; i < size.width(); i++)
            {
                double r{0}, g{0}, b{0};

                for(int k = 0; k < coeffs.size(); k++)
                {
                    int i0 = i + k - 3 * radius;

                    if(i0 < 0)
                    {
                        i0 = -i0;
                    }

                    if(i0 >= size.width())
                    {
                        i0 = 2 * size.width() - i0 - 1;
                    }

                    QRgb curr_color = src->pixel(i0, j);
                    r += qRed(curr_color) * coeffs[k];
                    g += qGreen(curr_color) * coeffs[k];
                    b += qBlue(curr_color) * coeffs[k];
                }

#pragma omp critical
                tempImg->setPixel(i, j, getColor(r, g, b));
            }
        }
#pragma omp barrier


        for(int j = thread_id; j < size.width(); j += N_thr)
        {
            for(int i = 0; i < size.height(); i++)
            {
                double r{0}, g{0}, b{0};

                for(int k = 0; k < coeffs.size(); k++)
                {
                    int i0 = i + k - 3 * radius;

                    if(i0 < 0)
                    {
                        i0 = -i0;
                    }

                    if(i0 >= size.height())
                    {
                        i0 = 2 * size.height() - i0 - 1;
                    }

                    QRgb curr_color = tempImg->pixel(j, i0);
                    r += qRed(curr_color) * coeffs[k];
                    g += qGreen(curr_color) * coeffs[k];
                    b += qBlue(curr_color) * coeffs[k];
                }

#pragma omp critical
                res->setPixel(j, i, getColor(r, g, b));
            }
        }
#pragma omp barrier
    }

    delete tempImg;
}

void gaussBlurSthr(QImage* src, QImage* res, int radius)
{
    if(radius == 0)
    {
        return;
    }

    QSize size = src->size();

    std::vector<double> coeffs;
    gaussCoefs(radius, coeffs);
    QImage* tempImg = new QImage(*src);


    for(int j = 0; j < size.height(); j++)
    {
        for(int i = 0; i < size.width(); i++)
        {
            double r{0}, g{0}, b{0};

            for(int k = 0; k < coeffs.size(); k++)
            {
                int i0 = i + k - 3 * radius;

                if(i0 < 0)
                {
                    i0 = -i0;
                }

                if(i0 >= size.width())
                {
                    i0 = 2 * size.width() - i0 - 1;
                }

                QRgb curr_color = src->pixel(i0, j);
                r += qRed(curr_color) * coeffs[k];
                g += qGreen(curr_color) * coeffs[k];
                b += qBlue(curr_color) * coeffs[k];
            }


            tempImg->setPixel(i, j, getColor(r, g, b));
        }
    }


    for(int j = 0; j < size.width(); j++)
    {
        for(int i = 0; i < size.height(); i++)
        {
            double r{0}, g{0}, b{0};

            for(int k = 0; k < coeffs.size(); k++)
            {
                int i0 = i + k - 3 * radius;

                if(i0 < 0)
                {
                    i0 = -i0;
                }

                if(i0 >= size.height())
                {
                    i0 = 2 * size.height() - i0 - 1;
                }

                QRgb curr_color = tempImg->pixel(j, i0);
                r += qRed(curr_color) * coeffs[k];
                g += qGreen(curr_color) * coeffs[k];
                b += qBlue(curr_color) * coeffs[k];
            }


            res->setPixel(j, i, getColor(r, g, b));
        }
    }

    delete tempImg;
}

//counting coefficients for motion blur
void motionCoefs(int radius, std::vector<double> &coefs)
{
    int r = abs(radius);
    coefs.resize(r);
    double s{0};

    if(radius > 0)
    {
        for(int i = 0; i < coefs.size(); i++)
        {
            coefs[i] = (double)i / r;
            s += coefs[i];
        }

        for(int i = 0; i < coefs.size(); i++)
        {
            coefs[i] /= s;
        }
    }
    else
    {
        for(int i = 0; i < coefs.size(); i++)
        {
            coefs[i] = ((double)r - i) / r;
            s += coefs[i];
        }

        for(int i = 0; i < coefs.size(); i++)
        {
            coefs[i] /= s;
        }
    }
}

void motionBlurMthr(QImage* src, QImage* res, int radius, bool horizontal)
{
    QSize size = src->size();

    std::vector<double> coeffs;
    motionCoefs(radius, coeffs);

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int N_thr = omp_get_num_threads();

        if(horizontal)
        {
            for(int j = thread_id; j < size.height(); j += N_thr)
            {
                for(int i = 0; i < size.width(); i++)
                {
                    double r{0}, g{0}, b{0};

                    for(int k = 0; k < coeffs.size(); k++)
                    {
                        int k0 = k;
                        if(radius > 0)
                        {
                            k0 = coeffs.size() - 1 - k;
                        }

                        int i0 = i - k0;
                        if(i0 < 0 || i0 >= size.width())
                        {
                            i0 = i;
                        }

                        QRgb curr_color = src->pixel(i0, j);
                        r += qRed(curr_color) * coeffs[k0];
                        g += qGreen(curr_color) * coeffs[k0];
                        b += qBlue(curr_color) * coeffs[k0];
                    }

#pragma omp critical
                    res->setPixel(i, j, getColor(r, g, b));
                }
            }
        }
        else
        {
            for(int j = thread_id; j < size.width(); j += N_thr)
            {
                for(int i = 0; i < size.height(); i++)
                {
                    double r{0}, g{0}, b{0};

                    for(int k = 0; k < coeffs.size(); k++)
                    {
                        int k0 = k;
                        if(radius > 0)
                        {
                            k0 = coeffs.size() - 1 - k;
                        }

                        int i0 = i - k0;
                        if(i0 < 0 || i0 >= size.height())
                        {
                            i0 = i;
                        }

                        QRgb curr_color = src->pixel(j, i0);
                        r += qRed(curr_color) * coeffs[k0];
                        g += qGreen(curr_color) * coeffs[k0];
                        b += qBlue(curr_color) * coeffs[k0];
                    }

#pragma omp critical
                    res->setPixel(j, i, getColor(r, g, b));
                }
            }
        }
#pragma omp barrier
    }

}

void motionBlurSthr(QImage* src, QImage* res, int radius, bool horizontal)
{
    QSize size = src->size();

    std::vector<double> coeffs;
    motionCoefs(radius, coeffs);

    {
        if(horizontal)
        {
            for(int j = 0; j < size.height(); j++)
            {
                for(int i = 0; i < size.width(); i++)
                {
                    double r{0}, g{0}, b{0};

                    for(int k = 0; k < coeffs.size(); k++)
                    {
                        int k0 = k;
                        if(radius > 0)
                        {
                            k0 = coeffs.size() - 1 - k;
                        }

                        int i0 = i - k0;
                        if(i0 < 0 || i0 >= size.width())
                        {
                            i0 = i;
                        }

                        QRgb curr_color = src->pixel(i0, j);
                        r += qRed(curr_color) * coeffs[k0];
                        g += qGreen(curr_color) * coeffs[k0];
                        b += qBlue(curr_color) * coeffs[k0];
                    }

                    res->setPixel(i, j, getColor(r, g, b));
                }
            }
        }
        else
        {
            for(int j = 0; j < size.width(); j++)
            {
                for(int i = 0; i < size.height(); i++)
                {
                    double r{0}, g{0}, b{0};

                    for(int k = 0; k < coeffs.size(); k++)
                    {
                        int k0 = k;
                        if(radius > 0)
                        {
                            k0 = coeffs.size() - 1 - k;
                        }

                        int i0 = i - k0;
                        if(i0 < 0 || i0 >= size.height())
                        {
                            i0 = i;
                        }

                        QRgb curr_color = src->pixel(j, i0);
                        r += qRed(curr_color) * coeffs[k0];
                        g += qGreen(curr_color) * coeffs[k0];
                        b += qBlue(curr_color) * coeffs[k0];
                    }

                    res->setPixel(j, i, getColor(r, g, b));
                }
            }
        }
    }

}

