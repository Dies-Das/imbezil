
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include "argparse.hpp"
#include <omp.h>

#include <cmath>
#include <initializer_list>

#include <iostream>
#include <cstdlib>
#include <utility>
#define THREAD_NUM 16
using namespace cv;

struct MyArgs : public argparse::Args {
    std::string &src_path = arg("Input image");
    int &n                = kwarg("n", "Number of parallel Shape insertions").set_default(16);
    int &m          = kwarg("m", "Number of candidate Shapes ").set_default(2000);
    int &iter = kwarg("k", "Number of iterations").set_default(200);
    std::string &o = kwarg("o", "output file path").set_default("out.png");
};

int main(int argc, char* argv[])
{
    MyArgs args = argparse::parse<MyArgs>(argc, argv);
    omp_set_num_threads(args.n);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> nor(0, 4);
    std::uniform_int_distribution<> newcoord(0,256);
    int threads = args.n;
    Mat image, image2, origimage;
    origimage = imread(args.src_path);

    resize(origimage,image,Size(256, 256));
    image.copyTo(image2);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    auto avg = mean(image);
    std::cout << image.size << std::endl;
    image2.setTo(Scalar(avg));
    std::vector<Rectangle> rects;
    std::vector<Rectangle> tries;
   int iter = 0;
    for(int k=0; k<args.m; k++){
        
        tries.push_back(Rectangle(rd, image, image2));
    }
    std::sort(std::begin(tries),std::end(tries));
    for(int k=0; k<threads; k++){
        rects.push_back(tries[k]);

    }
    for (int j=0; j<args.iter; j++){
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
    for(int k=0; k<args.m; k++){
        tries[k].randomize();
    }
    std::sort(std::begin(tries),std::end(tries));
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