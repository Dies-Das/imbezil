#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <random>
#include <cmath>
using namespace cv;
class Shape{
    public:

    virtual void draw( Mat  &image) = 0;
    virtual void undo() = 0;
    virtual void optimize() = 0;
};


class Rectangle:public Shape{
    public:
    Mat target;
    Mat current;
    int maxwidth;
    int maxheight;
    double fitness;
    Point2f tl;
    Point2f br;
    Point2f prevt;
    Point2f prevb;
    Scalar cl;
    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937 gen;
    void draw( Mat  &image);
    void undo();
    void mutate();
    void optimize();
    void randomize();
    double value();
    inline void updateColor();
    Rectangle( std::random_device &dev, const Mat & target, Mat &current){
            static thread_local std::mt19937  gen{dev()};
    std::normal_distribution<> nor(0, 5);
    std::uniform_int_distribution<> newcoord(0,256);
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = d;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[0];
        this->maxheight = shape[1];
        this->tl = Point2f(newcoord(gen),newcoord(gen));
        this->br = Point2f(newcoord(gen),newcoord(gen));
        this->draw(current2);
        this->fitness = norm(target, current2);
        
    };
};

