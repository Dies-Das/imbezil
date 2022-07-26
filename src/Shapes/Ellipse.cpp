#pragma once

#include "Ellipse.hpp"
#include <cstdlib>
using namespace cv;
const double alpha=0.7;
inline int clamp(double val, int lb, int ub){
    return (int)MAX(MIN(val,ub), 0);
}

inline void Ellipse::updateColor(){
    int tlx, tly, wid, hei;
    tlx = clamp(this->center.x-this->axes.width,0,this->maxwidth);
    tly = clamp(this->center.y-this->axes.height,0,this->maxheight);
    wid = clamp(2*this->axes.width,0,this->maxwidth-tlx);
    hei = clamp(2*this->axes.height,0,this->maxheight-tly);
    
    this->cl = mean(Mat(this->target, Rect(tlx,tly,wid,hei )));
}
void Ellipse::draw(Mat  &image){
    Mat overlay;
    image.copyTo(overlay);
    ellipse(image, this->center,this->axes,0,0,360, cl, FILLED);
    addWeighted(overlay, alpha, image, 1-alpha, 0, image);

}
void Ellipse::undo(){
    this->center = this->prevcenter;
    this->axes = this->prevaxes;
}
void Ellipse::mutate(){
    
    bool which;
    this->prevcenter = this->center;
    this->prevaxes = this->axes;
    which = rand() % 2;
    if (which){
    this->center.x += clamp(this->d(this->gen)+this->center.x,1, this->maxwidth-1);
    this->center.y += clamp(this->d(this->gen)+this->center.y,1, this->maxheight-1);
    }
    else{
    this->axes.width += clamp(this->d(this->gen)/5+this->axes.width, 1, this->maxwidth/2);
    this->axes.height += clamp(this->d(this->gen)/5+this->axes.height, 1, this->maxheight/2);
    }
this->updateColor();


}

void Ellipse::randomize(){
        Mat current2;
        current.copyTo(current2);
        std::uniform_int_distribution<> newcoord(0,256);
        std::uniform_int_distribution<> newsize(1, 4);
        this->center = Point2f(newcoord(gen),newcoord(gen));
        this->axes = Size(newsize(gen),newsize(gen));
        this->updateColor();
        this->draw(current2);

        this->fitness = norm(target, current2, NORM_L2);
}