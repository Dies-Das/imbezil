#pragma once

#include "Triangle.hpp"
#include <cstdlib>
using namespace cv;
const double alpha = 0.5;
int clamp(double val, int lb, int ub)
{
    return (int)MAX(MIN(val, ub), lb);
}

inline void Triangle::updateColor()
{
    Mat mask = Mat::zeros(this->target.rows, this->target.cols, CV_8U);
    fillConvexPoly(mask, &(this->pts[0]), 3, Scalar(1), 8, 0);

    this->cl = mean(this->target, mask);
}
void Triangle::draw(Mat &image)
{
    Mat overlay;
    image.copyTo(overlay);
    // this->cl = Scalar(255, 255, 255);
    fillConvexPoly(image, &(this->pts[0]), 3, this->cl, 8, 0);
    //            std::cout << "Color: " << this->cl << "\n";
    addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
    /*            std::cout << "Color: " << this->cl << std::endl;
            std::cout << "center: " << this->center << std::endl;
            std::cout << "ax: " << this->axes << std::endl;
           */
}
void Triangle::undo()
{
    this->pts = this->prevpts;
    this->cl = this->prevcl;
}
void Triangle::mutate()
{
    std::uniform_int_distribution<> newsize(1, 32);
    std::uniform_int_distribution<> wh(0, 3);

    bool which;
    this->prevpts = this->pts;
    this->prevcl = this->cl;
    which = wh(this->gen);

    this->pts[which].x = clamp(this->d(this->gen) + this->pts[which].x, 0, this->maxwidth - 1);
    this->pts[which].y = clamp(this->d(this->gen) + this->pts[which].y, 0, this->maxheight - 1);
    this->updateColor();
}

void Triangle::randomize()
{
    Mat current2;
    current.copyTo(current2);
     std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
    std::uniform_int_distribution<> next(-16, 17);
    this->pts[0] = Point(newcoordx(gen), newcoordy(gen));
    for (int k = 1; k < 3; k++)
    {
        this->pts[k] = Point(clamp(this->pts[0].x + next(gen), 0, this->maxwidth-1), clamp(this->pts[0].y + next(gen), 0, this->maxheight-1));
    }

    this->updateColor();
    this->draw(current2);

    this->fitness = norm(target, current2, NORM_L2);
}