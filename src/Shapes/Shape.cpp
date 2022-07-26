#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include <cstdlib>
#include <omp.h>

using namespace cv;
const double alpha=0.7;
inline int clamp(double val, int ub){
    return (int)MAX(MIN(val,ub), 0);
}
inline void Rectangle::updateColor(){
    this->cl = mean(Mat(this->target, Rect(this->tl, this->br)));
}
void Rectangle::draw(Mat  &image){
    Mat overlay;
    image.copyTo(overlay);
    rectangle(image, tl, br, cl, FILLED);
    addWeighted(overlay, alpha, image, 1-alpha, 0, image);

}
void Rectangle::undo(){
    this->br = this->prevb;
    this->tl = this->prevt;
    this->cl = this->prevcl;
}
void Rectangle::mutate(){
    bool which;
    this->prevb = this->br;
    this->prevt = this->tl;
    this->prevcl = this->cl;
    which = rand() % 2;
    if (which){
    this->br.x = clamp(this->d(this->gen)+this->br.x, this->maxwidth-1);
    this->br.y = clamp(this->d(this->gen)+this->br.y, this->maxwidth-1);
    }
    else{
    this->tl.x = clamp(this->d(this->gen)+this->tl.x, this->maxwidth-1);
    this->tl.y = clamp(this->d(this->gen)+this->tl.y, this->maxwidth-1);
    }
this->updateColor();


}

void Shape::optimize(){
    Mat current2;
    bool isOptimal = false;
    int numberoftries;

    while(!isOptimal){
        current.copyTo(current2);
        
        this->mutate();
        this->draw(current2);
        double val = norm(target, current2, NORM_L2);
        if(val < this->fitness){
            numberoftries = 0;
            this->fitness = val;
            
        }
        else{
            this->undo();
            numberoftries++;
            if(numberoftries > 10){
                isOptimal = true;
                break;
            }
                
        }
    }
            
    #pragma omp critical
    this->draw(current);
}
void Rectangle::randomize(){
        Mat current2;
        current.copyTo(current2);
        std::uniform_int_distribution<> newcoord(0,256);
        this->tl = Point2f(clamp(newcoord(gen), this->maxwidth),clamp(newcoord(gen), this->maxheight));
        this->br = Point2f(clamp(newcoord(gen), this->maxwidth),clamp(newcoord(gen), this->maxheight));
        this->updateColor();
        this->draw(current2);

        this->fitness = norm(target, current2, NORM_L2);
}