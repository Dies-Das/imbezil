
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include "Ellipse.hpp"
#include "argparse.hpp"
#include <omp.h>

#include <cmath>
#include <initializer_list>

#include <iostream>
#include <cstdlib>
#include <utility>
using namespace cv;
struct MyArgs : public argparse::Args
{
    std::string &src_path = kwarg("i","Input image").set_default("../testtest.png");
    int &n = kwarg("n", "Number of parallel Shape insertions").set_default(16);
    int &m = kwarg("m", "Number of candidate Shapes ").set_default(2000);
    int &iter = kwarg("k", "Number of iterations").set_default(200);
    int &sh = kwarg("s", "Shape, 1 is Rectangles, 2 is Ellipses").set_default(2);
    std::string &o = kwarg("o", "output file path").set_default("out.png");
};

template <typename T>
void approx(Mat &target, Mat &current, Mat &targetint, const MyArgs &args, std::random_device &rd)
{
    std::vector<T> rects;
    std::vector<T> tries;
    auto threads = args.n;
    int iter = 0;
    for (int k = 0; k < args.m; k++)
    {

        tries.push_back(T(rd, target, current, &targetint));
    }
    std::sort(std::begin(tries), std::end(tries));
    for (int k = 0; k < threads; k++)
    {
        rects.push_back(tries[k]);
    }
    for (int j = 0; j < args.iter; j++)
    {
        iter++;

#pragma omp parallel for
        for (int k = 0; k < threads; k++)
        {
            rects[k].optimize();
        }
        for (auto rect : rects)
        {
            rect.draw(current);
        }
        //    #pragma omp parallel
#pragma omp parallel for
        for (int k = 0; k < args.m; k++)
        {
            tries[k].randomize();
        }
        std::sort(std::begin(tries), std::end(tries));
        for (auto rec : tries)
        {
        }

        for (int k = 0; k < threads; k++)
        {
            rects[k] = tries[k];
        }
    }
}
int main(int argc, char *argv[])
{
    MyArgs args = argparse::parse<MyArgs>(argc, argv);
    omp_set_num_threads(args.n);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> nor(0, 4);
    std::uniform_int_distribution<> newcoord(0, 256);
    int threads = args.n;
    Mat image, image2, origimage, targetint;
    origimage = imread(args.src_path);

    resize(origimage, image, Size(256, 256));
    image.copyTo(image2);
    integral(image, targetint);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    auto avg = mean(image);
    std::cout << image.size << std::endl;
    image2.setTo(Scalar(avg));
    std::cout << "Type is: " << args.sh << std::endl;
    switch (args.sh)
    {
    case 1:
        approx<Rectangle>(image, image2, targetint, args, rd);
        std::cout << "doing rects for some reason??" << std::endl;
        break;
    case 2:
        approx<Ellipse>(image, image2, targetint, args, rd);
        break;
    }
    Mat dst;
    std::cout << norm(image2, image) << std::endl;
    resize(image2, dst, Size(), 4, 4, INTER_CUBIC);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", dst);
    waitKey(0);
    return 0;
}