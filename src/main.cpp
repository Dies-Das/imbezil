
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include <omp.h>
#include <pagmo/problem.hpp>
#include <pagmo/types.hpp>
#include <pagmo/algorithm.hpp>

#include <pagmo/algorithms/simulated_annealing.hpp>
#include <pagmo/algorithms/bee_colony.hpp>
#include <pagmo/algorithms/pso.hpp>
#include <pagmo/algorithms/sade.hpp>
#include <pagmo/algorithms/gaco.hpp>

#include <pagmo/archipelago.hpp>
#include <pagmo/population.hpp>
#include <cmath>
#include <initializer_list>
#include <pagmo/archipelago.hpp>
#include <iostream>
#include <cstdlib>
#include <utility>
#define THREAD_NUM 16
using namespace cv;
using namespace pagmo;
struct problem_v0
{
    Mat target;
    Mat current;
    vector_double fitness(const vector_double &dv) const
    {

        Mat current2;
        Mat overlay;
        double alpha = 0.5;
        Point2d P1(dv[0], dv[1]);
        Point2d P2(dv[2], dv[3]);
        Mat selection(target, Rect(P1, P2));
                Scalar col(mean(selection));
        //Scalar col(dv[4], dv[5], dv[6]);
        current.copyTo(current2);
        current.copyTo(overlay);
        rectangle(overlay, P1, P2, col, FILLED);
        addWeighted(overlay, alpha, current2, 1 - alpha, 0, current2);
        return {norm(target - current2)};
    }
    std::pair<vector_double, vector_double> get_bounds() const
    {
        auto a = target.size;
        double x = a[0];
        double y = a[1];
        return {{0., 0., 0., 0.}, {x, y, x, y}};
    }
};

int main()
{
    omp_set_num_threads(THREAD_NUM);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> nor(0, 4);
    std::uniform_int_distribution<> newcoord(0,256);
    int threads = 16;
    double alpha = 0.5;
    Mat image, image2, origimage;
    origimage = imread("../testtest.png");
    auto orig_size = origimage.size;
    resize(origimage,image,Size(256, 256));
    image.copyTo(image2);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    auto avg = mean(image);
    std::cout << image.size << std::endl;
    image2.setTo(Scalar(avg));
    std::vector<Rectangle> rects;
    std::vector<Rectangle> tries;
    auto comparison = [&](Rectangle &a, Rectangle &b){return a.fitness < b.fitness;};
    int iter = 0;
    for(int k=0; k<5000; k++){
        
        tries.push_back(Rectangle(rd, image, image2));
    }
    std::sort(std::begin(tries),std::end(tries),comparison);
    for(int k=0; k<threads; k++){
        rects.push_back(tries[k]);

    }
    for (int j=0; j<200; j++){
        iter++;
    
    #pragma omp parallel for
    for(int k=0; k<threads; k++){
        rects[k].optimize();
    }
    for(auto rect: rects){
        rect.draw(image2);
    }
//    #pragma omp parallel
    {
        #pragma omp parallel for
    for(int k=0; k<5000; k++){
        tries[k].randomize();
    }
    std::sort(std::begin(tries),std::end(tries),comparison);
    for(auto rec :tries){
        
    }

    for(int k=0; k<threads; k++){
        rects[k] =tries[k];

    }
    if(iter%10==0){

    }
    }

    }
    
    Mat dst;
    std::cout << norm(image2,image) << std::endl;
    resize(image2, dst, Size(),4,4,INTER_CUBIC);
        namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", dst);
    waitKey(0);
    return 0;
}