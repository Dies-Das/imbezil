#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include <cstdlib>
using namespace cv;

inline int clamp(double val, int ub){
    return (int)MAX(MIN(val,ub), 0);
}
void Rectangle::draw(Mat  &image){
    rectangle(image, tl, br, cl, FILLED);

}
void Rectangle::undo(){
    this->br = this->prevb;
    this->tl = this->prevt;
    this->cl = this->prevcl;
}
void Rectangle::mutate(){
    this->prevb = this->br;
    this->prevt = this->tl;
    this->prevcl = this->cl;

    this->br.x = clamp(this->d(this->gen), this->maxwidth);
    this->br.y = clamp(this->d(this->gen), this->maxwidth);
    this->tl.x = clamp(this->d(this->gen), this->maxwidth);
    this->tl.x = clamp(this->d(this->gen), this->maxwidth);


}