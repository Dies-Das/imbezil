#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include <cstdlib>
#include <omp.h>

using namespace cv;
const double alpha = 0.7;

inline void Rectangle::updateColor()
{
    Mat mask = Mat::zeros(this->target.rows, this->target.cols, CV_8U);
    rectangle(mask, this->tl, this->br, Scalar(1), FILLED);

    this->cl = mean(this->target, mask);
}
void Rectangle::draw(Mat &image)
{
    Mat overlay;
    image.copyTo(overlay);
    rectangle(image, tl, br, cl, FILLED);
    addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
}
void Rectangle::undo()
{
    this->br = this->prevb;
    this->tl = this->prevt;
    this->cl = this->prevcl;
}
void Rectangle::mutate()
{
    bool which;
    this->prevb = this->br;
    this->prevt = this->tl;
    this->prevcl = this->cl;
    which = rand() % 2;
    if (which)
    {
        this->br.x = CLAMP2(this->d(this->gen) + this->br.x, this->maxwidth - 1);
        this->br.y = CLAMP2(this->d(this->gen) + this->br.y, this->maxwidth - 1);
    }
    else
    {
        this->tl.x = CLAMP2(this->d(this->gen) + this->tl.x, this->maxwidth - 1);
        this->tl.y = CLAMP2(this->d(this->gen) + this->tl.y, this->maxwidth - 1);
    }
    this->updateColor();
}

void Shape::optimize()
{
    Mat current2;
    bool isOptimal = false;
    int numberoftries;

    while (!isOptimal)
    {
        current.copyTo(current2);

        this->mutate();
        this->draw(current2);
        double val = norm(target, current2, NORM_L2);
        if (val < this->fitness)
        {
            numberoftries = 0;
            this->fitness = val;
        }
        else
        {
            this->undo();
            numberoftries++;
            if (numberoftries > 20)
            {
                isOptimal = true;
                break;
            }
        }
    }

#pragma omp critical
    this->draw(current);
}
void Rectangle::randomize()
{
    Mat current2;
    current.copyTo(current2);
    std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
    this->tl = Point2f(CLAMP2(newcoordx(gen), this->maxwidth-1), CLAMP2(newcoordy(gen), this->maxheight-1));
    this->br = Point2f(CLAMP2(newcoordx(gen), this->maxwidth-1), CLAMP2(newcoordy(gen), this->maxheight-1));
    this->updateColor();
    this->draw(current2);

    this->fitness = norm(target, current2, NORM_L2);
}