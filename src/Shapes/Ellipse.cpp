#pragma once

#include "Ellipse.hpp"
#include <cstdlib>
using namespace cv;
const double alpha = 0.7;
inline int clamp(double val, int lb, int ub)
{
    return (int)MAX(MIN(val, ub), lb);
}

inline void Ellipse::updateColor()
{
    std::vector<Point2d> ellipsepoints;

    int tlx, tly, brx, bry;
    tlx = clamp(this->center.x - this->axes.width, 0, this->maxwidth);
    tly = clamp(this->center.y - this->axes.height, 0, this->maxheight);
    brx = clamp(this->center.x + this->axes.width, 0, this->maxwidth);
    bry = clamp(this->center.y + this->axes.height, 0, this->maxheight);
    /*    Mat temp(Size(this->maxwidth, this->maxheight),CV_8UC1);
        temp = Scalar(0);
        ellipse(temp,this->center, this->axes, 0, 0, 360, Scalar(255),FILLED);
        this->cl = mean(Mat(this->target, boundingRect(temp)));*/
    this->cl = mean(Mat(this->target, Rect(Point2d(tlx, tly), Point2d(brx, bry))));
}
void Ellipse::draw(Mat &image)
{
    Mat overlay;
    image.copyTo(overlay);
    // this->cl = Scalar(255, 255, 255);
    ellipse(image, this->center, this->axes, 0, 0, 360, this->cl, FILLED);
    //            std::cout << "Color: " << this->cl << "\n";
    addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
    /*            std::cout << "Color: " << this->cl << std::endl;
            std::cout << "center: " << this->center << std::endl;
            std::cout << "ax: " << this->axes << std::endl;
           */
}
void Ellipse::undo()
{
    this->center = this->prevcenter;
    this->axes = this->prevaxes;
    this->cl = this->prevcl;
}
void Ellipse::mutate()
{
    std::uniform_int_distribution<> newsize(1, 32);
    bool which;
    this->prevcenter = this->center;
    this->prevaxes = this->axes;
    this->prevcl = this->cl;
    which = rand() % 2;
    if (which)
    {
        this->center.x = clamp(this->d(this->gen) + this->center.x, 1, this->maxwidth - 1);
        this->center.y = clamp(this->d(this->gen) + this->center.y, 1, this->maxheight - 1);
    }
    else
    {
        this->axes = Size(newsize(gen), newsize(gen));
    }
    this->updateColor();
}

void Ellipse::randomize()
{
    Mat current2;
    current.copyTo(current2);
    std::uniform_int_distribution<> newcoord(0, 255);
    std::uniform_int_distribution<> newsize(1, 32);
    this->center = Point2f(newcoord(gen), newcoord(gen));
    this->axes = Size(newsize(gen), newsize(gen));
    this->updateColor();
    this->draw(current2);

    this->fitness = norm(target, current2, NORM_L2);
}