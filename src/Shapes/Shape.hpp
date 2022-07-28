#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <random>
#include <cmath>
using namespace cv;
class Shape
{
public:
    Scalar cl;
    double fitness;
    Mat target;
    Mat current;
    Mat *targetint;
    virtual void draw(Mat &image) = 0;
    virtual void undo() = 0;
    virtual void mutate() = 0;
    void optimize();
    inline friend bool operator<(const Shape &l, const Shape &r)
    {
        return l.fitness < r.fitness;
    }
    inline friend bool operator>(const Shape &l, const Shape &r)
    {
        return r < l;
    }
    Shape(Mat *tint)
    {
        this->targetint = tint;
    };
};

class Rectangle final : public Shape
{
public:
    int maxwidth;
    int maxheight;
    Point2f tl;
    Point2f br;
    Point2f prevt;
    Point2f prevb;

    Scalar prevcl;
    std::normal_distribution<> d;
    std::mt19937 gen;
    void draw(Mat &image);
    void undo();
    void mutate();
    // void optimize();
    void randomize();
    double value();
    inline void updateColor();
    Rectangle(std::random_device &dev, const Mat &target, Mat &current, Mat *tint) : Shape(tint)
    {

        static thread_local std::mt19937 gen{dev()};
        std::normal_distribution<> nor(0, 5);
        std::uniform_int_distribution<> newcoord(0, 256);
        Mat current2;
        current.copyTo(current2);
        this->target = target;
        this->current = current;
        this->d = nor;
        this->gen = gen;
        auto shape = target.size;
        this->maxwidth = shape[0];
        this->maxheight = shape[1];
        this->tl = Point2f(newcoord(gen), newcoord(gen));
        this->br = Point2f(newcoord(gen), newcoord(gen));
        this->draw(current2);
        this->fitness = norm(target, current2, NORM_L2);
    };
};
