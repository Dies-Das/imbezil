#pragma once
#include "opencv4/opencv2/imgproc.hpp"
#include "Quadratic.hpp"
#include <cstdlib>
#include <omp.h>
using namespace cv;
const double alpha = 0.5;


void Bezier::updateColor()
{
    Mat mask = Mat::zeros(this->target.rows, this->target.cols, CV_8U);
    Mat curve(this->polypoints, true);
curve.convertTo(curve, CV_32S); //adapt type for polylines
polylines(mask, curve, false, Scalar(1), 1, LINE_AA);
//    for(int k=0; k<99; k++)
 //   line(mask, this->pts[0], this->pts[1], Scalar(1), 4, LINE_AA, 0);//this->polypoints[k],this->polypoints[k+1], Scalar(1), 2, LINE_AA, 0);

    this->cl = mean(this->target, mask);
}
void Bezier::draw(Mat &image)
{
    Mat overlay;
    image.copyTo(overlay);
        Mat curve(this->polypoints, true);
curve.convertTo(curve, CV_32S);
    // this->cl = Scalar(255, 255, 255);
      //  for(int k=0; k<99; k++)
    polylines(image, curve,false, this->cl, 1, LINE_AA);//line(image, this->polypoints[k],this->polypoints[k+1], this->cl, 2, LINE_AA, 0);

    //            std::cout << "Color: " << this->cl << "\n";
    addWeighted(overlay, alpha, image, 1 - alpha, 0, image);
    /*            std::cout << "Color: " << this->cl << std::endl;
            std::cout << "center: " << this->center << std::endl;
            std::cout << "ax: " << this->axes << std::endl;
           */
}
void Bezier::undo()
{
    this->pts = this->prevpts;
    this->cl = this->prevcl;
    this->polypoints = this->prevpoly;
}
void Bezier::mutate()
{
    std::uniform_int_distribution<> newsize(1, 32);
    std::uniform_int_distribution<> wh(0, 3);

    bool which;
    this->prevpts = this->pts;
    this->prevcl = this->cl;
    this->prevpoly = this->polypoints;
    which = wh(this->gen);

    this->pts[which].x = CLAMP(this->d(this->gen) + this->pts[which].x, 0, this->maxwidth - 1);
    this->pts[which].y = CLAMP(this->d(this->gen) + this->pts[which].y, 0, this->maxheight - 1);
    this->eval();
    this->updateColor();
}

void Bezier::randomize()
{
    Mat current2;
    current.copyTo(current2);
     std::uniform_int_distribution<> newcoordx(0, this->maxwidth);
    std::uniform_int_distribution<> newcoordy(0, this->maxheight);
    
    std::uniform_int_distribution<> next(-16, 17);
        for (int k = 0; k < 3; k++)
        {
            //this->pts[k] = Point(CLAMP(this->pts[0].x + next(gen), 0, this->maxwidth-1), CLAMP(this->pts[0].y + next(gen), 0, this->maxheight-1));
            this->pts[k] = Point(newcoordx(gen), newcoordy(gen));
        }
    this->eval();
    this->updateColor();
    this->draw(current2);

    this->fitness = norm(target, current2, NORM_L2);
}

void Bezier::eval(){
    double step = 1.0/49.0;
    #pragma omp simd
    for(int k=0; k<50; k++){

        this->polypoints[k].x = this->pts[1].x+((1-step*k)*(1-step*k)*(this->pts[0].x-this->pts[1].x))+step*step*k*k*(this->pts[2].x-this->pts[1].x);
         this->polypoints[k].y = this->pts[1].y+((1-step*k)*(1-step*k)*(this->pts[0].y-this->pts[1].y))+step*step*k*k*(this->pts[2].y-this->pts[1].y);
    }

}