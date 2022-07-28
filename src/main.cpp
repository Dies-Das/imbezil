
#include "opencv4/opencv2/opencv.hpp"
#include "Shape.hpp"
#include "Ellipse.hpp"
#include "Triangle.hpp"

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
    std::string &src_path = kwarg("i", "Input image").set_default("../testtest.jpg");
    int &n = kwarg("j", "Number of parallel Shape insertions").set_default(8);
    int &m = kwarg("m", "Number of candidate Shapes ").set_default(200);
    int &iter = kwarg("k", "Number of iterations").set_default(400);
    int &sh = kwarg("s", "Shape, 1 is Rectangles, 2 is Ellipses").set_default(3);
    int &size = kwarg("n", "approximate minimum width of subsampled image").set_default(256);
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

        /*
                for (auto rect : rects)
                {
                    rect.draw(current);
                }*/
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
    omp_set_num_threads(16);
    std::random_device rd{};
    

    Mat image, image2, origimage, targetint;
    origimage = imread(args.src_path);

    double scale = MIN((double)origimage.size[0]/args.size,(double)origimage.size[1]/args.size);

    resize(origimage, image, Size(), 1/scale, 1/scale, INTER_AREA);
    
    image.copyTo(image2);
    integral(image, targetint);

    auto avg = mean(image);

    image2.setTo(Scalar(avg));

    switch (args.sh)
    {
    case 1:
        approx<Rectangle>(image, image2, targetint, args, rd);
        break;
    case 2:
        approx<Ellipse>(image, image2, targetint, args, rd);
        break;
    case 3:
        approx<Triangle>(image, image2, targetint, args, rd);
    }
    Mat dst;

    resize(image2, dst, Size(), scale, scale, INTER_CUBIC);

    imwrite(args.o, dst);

    return 0;
}