#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <random>
#include <cmath>
using namespace cv;
class Shape{
    public:

    virtual void draw(Mat  &image) = 0;
    virtual void undo() = 0;
};
class Rectangle:public Shape{
    public:
    int maxwidth;
    int maxheight;
    Point2f tl;
    Point2f br;
    Point2f prevt;
    Point2f prevb;
    Scalar cl;
    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937 gen;
    void draw(Mat  &image);
    void undo();
    void mutate();
};